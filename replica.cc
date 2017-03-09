
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <fstream>
#include <thread>

#include "replica.h"
#include "message.h"

/* Setting up the replica with the provided port and host */
replica::replica(int _port, string _host, int _id, string _config_file) :
port(_port), host(_host), id(_id), cur_view_num(-1), net(_port, _host)
{
    string h, p, rep_id;
    ifstream config_fs(_config_file);
    while (config_fs >> h >> p >> rep_id) {
        // letting vector use move semantics underneath
        replicas.push_back(node(stoi(p), h));
    }
    num_replicas = replicas.size();
    acceptor.init(num_replicas, id);
    learner.init(num_replicas, id);
    proposer.init(replicas, id, &net);
}

/* Start listening on the provided port and host */
void replica::start() {
    while (true) {
        Message *msg = net.recv_from();
        thread t(&replica::handle_msg, this, msg);
        t.detach();
    }
}

/* Adds all the replicas in the system to the receiver list */
void replica::make_broadcast(Message *ref){
    for (auto& r : replicas) {
        ref->receivers.push_back(r);
    }
}

bool replica::is_primary(int view_num) {
    return cur_view_num % num_replicas == id && proposer.reached_quroum(view_num);
}

/* Handle the given message */
void replica::handle_msg(Message *message) {
    // The simple answer to all of your concurreny problems
    // unique_lock<mutex> lock(m);
    Message* reply = new Message();
    COUT << "Msg in handle_msg: " << message->serialize() << endl;
    COUT << "Current view is: " << cur_view_num << endl;
    switch (message->msg_type) {
        case MessageType::NO_ACTION:
            // do nothing in this case
            break;
        case MessageType::START_PREPARE:
        {
            std::string in_client_id = message->get_client_id();
            int in_client_seq_number = message->get_client_seq_num();

            // if the client's request is already learnt, but didn't get a response
            // send a response right away
            if(client_progress_map[in_client_id] >= in_client_seq_number){
                reply->msg_type = MessageType::PROPOSAL_LEARNT;
                reply->receivers.push_back(message->get_client_node());
                break;
            }

            // Various scenarios when control can come here
            // 1. first primary comes here  (message->view_num == cur_view_num
            // 2. view changed, next primary comes in  (message->view_num > cur_view_num)
            // 3. new primary finished fix and exec new client reqs (message->view_num == cur_view_num)
            //
            if (message->view_num > cur_view_num) {
                // scenario 2
                cur_view_num = message->view_num;
                if (cur_view_num % num_replicas == id) {
                    // Becoming the primary and fix previous sate
                    // TODO
                    this->proposer.is_new_primary = true;
                    string true_value = message->value 
                        + "#" + to_string(message->sender.port)
                        + "#" + message->sender.host;
                    proposer.to_propose = true_value;
                    reply = proposer.handle_start_prepare(cur_view_num);
                    reply->value = true_value;
                    make_broadcast(reply);
                    // store client sender info
                }
            }
            
            // If we already know that we are the primary
            if (is_primary(message->view_num)) {
                // Keep track of which seqnum maps to which client, so that when it is commited, we know who to tell
                int tmp_seq_num = learner.get_seqnum();
                seq_to_client_map[tmp_seq_num] = message->sender;

                // BIG BUG
                // cannot just call acceptor's propse_msg
                //reply = acceptor.accept_propose_msg(message->view_num, message->value, tmp_seq_num);

                // The fix :
                reply = proposer.handle_prepare_accept(message->acceptor_state, cur_view_num, message->value, tmp_seq_num);
                make_broadcast(reply);
                break;
            }
//            if (cur_view_num % num_replicas == id) {
//                // does this happen only for the very first primary?
//                // for all other primaries that come up, they are bound to enter the scenario 2
//                // and will definitely get a quorum right?
//
//                // assert(false);
//                // Check for holes
//                // add the initial value to be proposed to proposer state
//                proposer.to_propose = message->value;
//                reply = proposer.start_prepare(message->view_num);
//                make_broadcast(reply);
//            }
            break;
        }
        case MessageType::PREPARE_ACCEPT:
        {
            // TODO: when will the below handle_prepare be called - what should the seq number be?
            int dummy_seq = 99;
            reply = proposer.handle_prepare_accept(message->acceptor_state, message->view_num, message->value, dummy_seq);
            // add all the acceptors to the receiver list
            // Acceptor vector check
            make_broadcast(reply);
            break;
        }
        case MessageType::PREPARE_REJECT:
        {
            reply = proposer.handle_prepare_reject(message->view_num);
            // add all the acceptors to the receiver list (proposer will propose again)
            make_broadcast(reply);
            break;
        }
        case MessageType::PROPOSE_ACCEPT:
        {
            reply = proposer.handle_propose_accept(message->view_num);
            // nothing to be sent for now in this scenario
            break;
        }
        case MessageType::PROPOSE_REJECT:
        {
            reply = proposer.handle_propose_reject(message->view_num);
            // add all the acceptors to the receiver list (proposer will propose again)
            make_broadcast(reply);
            break;
        }
        // Acceptor scenarios
        case MessageType::PREPARE:
        {
            reply = acceptor.accept_prepare_msg(message->view_num);
            // add only the proposer to the receiver list
            reply->receivers.push_back(message->sender);
            break;
        }
        case MessageType::PROPOSE:
        {
            reply = acceptor.accept_propose_msg(message->view_num, message->value, message->seq_num);
            // add all the learners to the receiver list
            make_broadcast(reply);
            break;
        }
            // Learner scenarios
        case MessageType::BRDCST_LEARNERS:
        {
            reply = learner.update_vote(message->view_num, message->seq_num, message->value);
            // add the proposer to the receiver list
            make_broadcast(reply);
            //reply->receivers
            break;
        }
        case MessageType::PROPOSAL_LEARNT:{
            // Send the same msg that we just got, but just send it to the client

            // all replicas get this message
            std::string client_id = message->get_client_id();
            int client_seq_number = message->get_client_seq_num();
            client_progress_map[client_id] = client_seq_number;

            if (is_primary(message->view_num)) {
                // primary is responsible for sending it back to the client
                  //learner.broadcast_learn(message->seq_num);
                reply->msg_type = MessageType::PROPOSAL_LEARNT;
                reply->receivers.push_back(message->get_client_node());
            }
            break;
        }
    }
    delete(message);
    if (reply != nullptr && reply->msg_type != MessageType::NO_ACTION){
        reply->sender.host = host;
        reply->sender.port = port;
        net.sendto(reply);
        //COUT << "Sending out msg: " << reply->serialize() << endl;
        //net.sendto(reply);
        //this_thread::sleep_for(1s);
    }
    // Send msg if not type noaction
    
    // If client_req msg
    // skip
    
    // Incoming: prepare
    // Arguments: n
    // Acting as: Acceptor
    // Check if you've seen an n higher than this one
    // Outgoing: a prepare_accept
    //    OR prepare_reject
    
    // Incoming: prepare_accept
    // Arguments: n_a, v_a (could be null)
    // Acting as: Proposer
    // Increment the count of # of prepare accepts you have gotten
    // Outgoing: If this is more than a majority, send propose_value(n, v)
    
    // Incoming: prepare_reject
    // Arguments: n_p
    // Acting as: Proposer
    // If we got a higher n value, update our internal storage with this new value
    // Outgoing:
    
    // Incoming: propose
    // Arguments: n, v
    // Acting as: Acceptor
    // Outgoing: propose_accept(n) + broadcast_to_learners(n, v),
    //    OR propose_reject(n)
    
    // Incoming: propose_accept
    // Arguments: n
    // Acting as: Proposer
    // Outgoing:
    
    // Incoming: propose_reject
    // Arguments: n_p
    // Acting as: Proposer
    
    // Incoming: broadcast_to_learners
    // Arguments: n, v
    // Acting as: Learner
    // Outgoing: potenial client response
}
