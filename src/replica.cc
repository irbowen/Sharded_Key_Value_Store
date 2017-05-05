
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

#include "../headers/replica.h"
#include "../headers/message.h"

/* Setting up the replica with the provided port and host */
replica::replica(int _port, string _host, int _id, string _config_file, string _holes_file) :
    port(_port), host(_host), id(_id), cur_view_num(-1), net(_port, _host)
{
    string h, p, rep_id;
    ifstream config_fs(_config_file);
    while (config_fs >> h >> p >> rep_id) {
        replicas.push_back(node(stoi(p), h));
    }
    if (_holes_file != "") {
        string hole;
        ifstream holes_fs(_holes_file);
        while (holes_fs >> hole) {
            seq_holes.push_back(stoi(hole));
        }
    }
    num_replicas = replicas.size();
    acceptor.init(num_replicas, id);
    learner.init(num_replicas, id, port);
    proposer.init(replicas, id, &net, seq_holes);

    /* Set up the key value store object */
    kv_store_ = make_unique<KV_Store>(id, port, host, replicas);
    kv_store_->init(&learner);
}

void replica::recv() {
    while (true) {
        Message* msg = net.recv_from();
        handle_msg(msg);
    }
}

/* Start listening on the provided port and host */
void replica::start() {
    this->recv();
}

/* Adds all the replicas in the system to the receiver list */
void replica::make_broadcast(Message* ref) {
    for (auto& r: replicas) {
        ref->receivers.push_back(r);
    }
}

bool replica::is_primary(int view_num) {
    return cur_view_num % num_replicas == id && proposer.reached_quroum(view_num);
}

bool replica::is_previous_view(int view_num) {
    return seen_views.count(view_num) == 1;
}


/* Handle the given message */
void replica::handle_msg(Message* message) {
    // The simple answer to all of your concurreny problems
    // unique_lock<mutex> lock(m);
    Message* reply = new Message();
    // COUT <<"Msg in handle_msg: " <<message->serialize() <<endl;
    // COUT <<"Current view is: " <<cur_view_num <<endl;
    switch (message->msg_type) {
        case MessageType::NO_ACTION:
            break;
        case MessageType::START_PREPARE: {
            std::string in_client_id = message->get_client_id();
            int in_client_seq_number = message->get_client_seq_num();

            // if the client's request is already learnt, but didn't get a response
            // send a response right away
            if(client_progress_map.count(in_client_id) != 0 &&
                client_progress_map[in_client_id] >= in_client_seq_number){
                reply->key = message->key;
                reply->value = learner.get_latest_value(message->key);
                reply->msg_type = MessageType::PROPOSAL_LEARNT;
                reply->receivers.push_back(message->get_client_node());
                break;
            }
            proposer.to_propose = message->value;
            if (message->view_num > cur_view_num) {
                cur_view_num = message->view_num;
                if (cur_view_num % num_replicas == id) {
                    this->proposer.is_new_primary = true;
                    reply = proposer.handle_start_prepare(cur_view_num);
                    reply->value = message->value;
                    make_broadcast(reply);
                }
            }
            if (is_primary(message->view_num)) {
                int tmp_seq_num = learner.get_seqnum();
                seq_to_client_map[tmp_seq_num] = message->sender;
                reply = proposer.handle_prepare_accept_fast(
                        message->acceptor_state, cur_view_num, message->value, tmp_seq_num);
                make_broadcast(reply);
            }
            break;
        }
        case MessageType::PREPARE_ACCEPT: {
            int dummy_seq = 0;
            reply = proposer.handle_prepare_accept(
                    message->acceptor_state, message->view_num, message->value, dummy_seq);
            make_broadcast(reply);
            break;
        }
        case MessageType::PREPARE_REJECT: {
            reply = proposer.handle_prepare_reject(message->view_num);
            make_broadcast(reply);
            break;
        }
        case MessageType::PROPOSE_ACCEPT: {
            reply = proposer.handle_propose_accept(message->view_num);
            break;
        }
        case MessageType::PROPOSE_REJECT: {
            reply = proposer.handle_propose_reject(message->view_num);
            make_broadcast(reply);
            break;
        }
        /* Acceptors handle prepare and propose messages */
        case MessageType::PREPARE: {
            reply = acceptor.accept_prepare_msg(message->view_num);
            reply->receivers.push_back(message->sender);
            break;
        }
        case MessageType::PROPOSE: {
            reply = acceptor.accept_propose_msg(message->view_num, message->value, message->seq_num);
            make_broadcast(reply);
            break;
        }
        /* Accept messages are hanlded by the learner */
        case MessageType::ACCEPT_VALUE: {
            reply = learner.handle_learn_msg(message->view_num, message->seq_num, message->value, message);
            make_broadcast(reply);
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
        case MessageType::PUT: {
            reply = kv_store_->handle_put_msg(message);
            break;
        }
        case MessageType::GET: {
            reply = kv_store_->handle_get_msg(message);
            break;
        }
        case MessageType::GET_KEYS: {
            reply = kv_store_->handle_get_all_keys(message);
            break;
        }
    }
    if (reply != nullptr && reply->msg_type != MessageType::NO_ACTION) {
        reply->sender.host_ = host;
        reply->sender.port_ = port;
        // cout << "Sending reply: " << reply->serialize() << endl;
        net.sendto(reply);
    }
    delete(message);
    delete(reply);
}
