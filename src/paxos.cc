
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>

#include "../headers/paxos.h"
#include "../headers/message.h"

/* Setting up the replica with the provided port and host */
Paxos::Paxos(Environment* env, string holes_file) : cur_view_num(-1), env_(env) {
    if (holes_file != "") {
        string hole;
        ifstream holes_fs(holes_file);
        while (holes_fs >> hole) {
            seq_holes.push_back(stoi(hole));
        }
    }
    acceptor_.init(env);
    learner_.init(env);
    proposer_.init(env, seq_holes);
}

Learner* Paxos::get_learner() {
  return &learner_;
}

bool Paxos::is_primary(int view_num) {
    return cur_view_num % env_->num_replicas_ == env_->replica_id_ && proposer_.reached_quroum(view_num);
}

bool Paxos::is_previous_view(int view_num) {
    return seen_views.count(view_num) == 1;
}

/* Handle the given message */
Message* Paxos::handle_msg(Message* message) {
    Message* reply = new Message();
    switch (message->msg_type) {
        case MessageType::NO_ACTION:
            break;
        case MessageType::START_PREPARE: {
            std::string in_client_id = message->get_client_id();
            int in_client_seq_number = message->get_client_seq_num();
            // if the client's request is already learnt, but didn't get a response
            // send a response right away
            if (client_progress_map.count(in_client_id) != 0 &&
                client_progress_map[in_client_id] >= in_client_seq_number) {
                reply->key = message->key;
                reply->value = learner_.get_latest_value(message->key, message->column);
                reply->msg_type = MessageType::PROPOSAL_LEARNT;
                reply->receivers.push_back(message->get_client_node());
                break;
            }
            proposer_.to_propose = message->value;
            if (message->view_num > cur_view_num) {
                cur_view_num = message->view_num;
                if (cur_view_num % env_->num_replicas_ == env_->replica_id_) {
                    proposer_.is_new_primary = true;
                    reply = proposer_.handle_start_prepare(cur_view_num);
                    reply->value = message->value;
                    env_->convert_msg_to_broadcast(reply);
                    break;
                }
            }
            if (is_primary(message->view_num)) {
                int tmp_seq_num = learner_.get_seqnum();
                seq_to_client_map[tmp_seq_num] = message->sender;
                reply = proposer_.handle_prepare_accept_fast(
                        message->acceptor_state, cur_view_num, message->value, tmp_seq_num);
                env_->convert_msg_to_broadcast(reply);
            }
            break;
        }
        case MessageType::PREPARE_ACCEPT: {
            int dummy_seq = 0;
            reply = proposer_.handle_prepare_accept(
                    message->acceptor_state, message->view_num, message->value, dummy_seq);
            env_->convert_msg_to_broadcast(reply);
            break;
        }
        case MessageType::PREPARE_REJECT: {
            reply = proposer_.handle_prepare_reject(message->view_num);
            env_->convert_msg_to_broadcast(reply);
            break;
        }
        case MessageType::PROPOSE_ACCEPT: {
            reply = proposer_.handle_propose_accept(message->view_num);
            break;
        }
        case MessageType::PROPOSE_REJECT: {
            reply = proposer_.handle_propose_reject(message->view_num);
            env_->convert_msg_to_broadcast(reply);
            break;
        }
        /* Acceptors handle prepare and propose messages */
        case MessageType::PREPARE: {
            reply = acceptor_.accept_prepare_msg(message->view_num);
            reply->receivers.push_back(message->sender);
            break;
        }
        case MessageType::PROPOSE: {
            reply = acceptor_.accept_propose_msg(message->view_num, message->value, message->seq_num);
            env_->convert_msg_to_broadcast(reply);
            break;
        }
        /* Accept messages are hanlded by the learner */
        case MessageType::ACCEPT_VALUE: {
            reply = learner_.handle_learn_msg(message->view_num, message->seq_num, message->value, message);
            env_->convert_msg_to_broadcast(reply);
            break;
        }
        /* Value learned msg's are handled by the primary, and sent on to the client */
        case MessageType::PROPOSAL_LEARNT: {
            std::string client_id = message->get_client_id();
            int client_seq_number = message->get_client_seq_num();
            if (is_primary(message->view_num)) {
                if(client_progress_map.count(client_id) != 0 &&
                   client_progress_map[client_id] >= client_seq_number)
                    break;
                reply->msg_type = MessageType::PROPOSAL_LEARNT;
                reply->value = message->value;
                reply->receivers.push_back(message->get_client_node());
            }
            client_progress_map[client_id] = client_seq_number;
            break;
        }
    }
    if (env_->is_debug()) { cout << "Paxos has created this msg: " << reply->serialize() << endl; }
    return reply;
}
