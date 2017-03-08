
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
port(_port), host(_host), id(_id), cur_view_num(0), net(_port, _host)
{
    string h, p, rep_id;
    ifstream config_fs(_config_file);
    while (config_fs >> h >> p >> rep_id) {
        // letting vector use move semantics underneath
        replicas.push_back(node(stoi(p), h));
    }
    num_replicas = replicas.size();
    acceptor.init(0, num_replicas, id);
    learner.init(num_replicas, id);
    proposer.init(replicas, id);
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
void replica::add_all_to_receiver_list(Message *ref){
    for (auto& r : replicas) {
        ref->receivers.push_back(r);
    }
}

/* Handle the given message */
void replica::handle_msg(Message *message) {
    Message* reply = new Message();
    cout << "Msg in handle_msg: " << message->serialize() << endl;
    cout << "Msg type: " << message->msg_type << endl;
    switch (message->msg_type) {
        case MessageType::NO_ACTION:
            // do nothing in this case
            break;
        case MessageType::START_PREPARE:
        {
            // If we are the primary, then we should send a
            // start prepare message to all other replicas
            if (cur_view_num % num_replicas == id) {
                // add the initial value to be proposed to proposer state
                proposer.to_propose = message->value;
                reply = proposer.start_prepare(message->view_num, learner.get_seqnum());
                add_all_to_receiver_list(reply);
            }
            // Otherwise, we need to increment our viewnum and assert it
            // is equal to the viewnum sent in the client msg
            else {
                cur_view_num += 1;
            }
            break;
        }
        case MessageType::PREPARE_ACCEPT:
        {
            reply = proposer.prepare_accept(message->view_num, message->value);
            // add all the acceptors to the receiver list
            add_all_to_receiver_list(reply);
            break;
        }
        case MessageType::PREPARE_REJECT:
        {
            reply = proposer.prepare_reject(message->view_num, message->seq_num);
            // add all the acceptors to the receiver list (proposer will propose again)
            add_all_to_receiver_list(reply);
            break;
        }
        case MessageType::PROPOSE_ACCEPT:
        {
            reply = proposer.propose_accept(message->view_num);
            // nothing to be sent for now in this scenario
            break;
        }
        case MessageType::PROPOSE_REJECT:
        {
            reply = proposer.propose_reject(message->view_num, message->seq_num);
            // add all the acceptors to the receiver list (proposer will propose again)
            add_all_to_receiver_list(reply);
            break;
        }
            // Acceptor scenarios
        case MessageType::PREPARE:
        {
            reply = acceptor.prepare(message->view_num);
            // add only the proposer to the receiver list
            reply->receivers.push_back(message->sender);
            break;
        }
        case MessageType::PROPOSE:
        {
            reply = acceptor.propose(message->view_num, message->value);
            // add all the learners to the receiver list
            add_all_to_receiver_list(reply);
            break;
        }
            // Learner scenarios
        case MessageType::BRDCST_LEARNERS:
        {
            reply = learner.update_vote(message->view_num, message->seq_num, message->value);
            // add the proposer to the receiver list
            reply->receivers.push_back(message->sender);
            break;
        }
        case MessageType::PROPOSAL_LEARNT:
        {
            // TODO: return response back to the proposer/client
            break;
        }
    }
    delete(message);
    if (reply != nullptr && reply->msg_type != MessageType::NO_ACTION){
        reply->sender.host = host;
        reply->sender.port = port;
        net.sendto(reply);
        cout << "Sending out msg: " << reply->serialize() << endl;
        net.sendto(reply);
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
