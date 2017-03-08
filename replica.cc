
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
#include "json.h"
#include "message.h"

using namespace std;

/* Setting up the replica with the provided port and host */
replica::replica(int _port, string _host, int _id, string _config_file)
: net(_host, _port), learner(4) {

    this->port = _port;
    this->host = _host;
    this->my_id = _id;
    cur_view_num = 0;

    ifstream config_fs;
    config_fs.open(_config_file);
    // TODO
    // Read from config file
    // Determine number of replicas
    // pass that to learner
    tot_replicas = 3;
}

/* Start listening on the provided port and host */
void replica::start() {
    while (true) {
        Message *msg = net.recv_from();
        thread t(&replica::handle_msg, this, msg);
        t.detach();
    }
}
void replica::add_all_to_receiver_list(Message *ref){
    // TEST
    ref->receivers.push_back(node(2000, LOCALHOST));
    ref->receivers.push_back(node(2001, LOCALHOST));
    ref->receivers.push_back(node(2002, LOCALHOST));
}
/* Handle the given message */
void replica::handle_msg(Message *message) {
    cout << "Got: " << message->value << endl;

    Message *reply = new Message;
    // By default, any message sent back must sender info
    reply->sender.host = host;
    reply->sender.port = port;

    switch(message->msg_type){
        case MessageType::NO_ACTION:
            break;
        // Proposer scenarios
        case MessageType::START_PREPARE:{
            if(cur_view_num % tot_replicas == my_id)
                reply = proposer.start_prepare(message->prop_number);
            else
                // this assumes the first message is not broadcasted but sent to only machine 0
                cur_view_num += 1;
            break;
        }
        case MessageType::PREPARE_ACCEPT:{
            reply = proposer.prepare_accept(message->n_a, message->value);
            // add all the acceptors to the receiver list
            add_all_to_receiver_list(reply);
            break;
        }
        case MessageType::PREPARE_REJECT:{
            reply = proposer.prepare_reject(message->n_p);
            // add all the acceptors to the receiver list (proposer will propose again)
            add_all_to_receiver_list(reply);
            break;
        }
        case MessageType::PROPOSE_ACCEPT:{
            reply = proposer.propose_accept(message->prop_number);
            // nothing to be sent for now in this scenario
            break;
        }
        case MessageType::PROPOSE_REJECT:{
            reply = proposer.propose_reject(message->n_p);
            // add all the acceptors to the receiver list (proposer will propose again)
            add_all_to_receiver_list(reply);
            break;
        }
        // Acceptor scenarios
        case MessageType::PREPARE:{
            reply = acceptor.prepare(message->prop_number);
            // add only the proposer to the receiver list
            reply->receivers.push_back(message->sender);
            break;
        }
        case MessageType::PROPOSE:{
            reply = acceptor.propose(message->prop_number, message->value);
            // add all the learners to the receiver list
            add_all_to_receiver_list(reply);
            break;
        }
        // Learner scenarios
        case MessageType::BRDCST_LEARNERS:{
            reply = learner.update_vote(message->n_a, message->value);
            // add the proposer to the receiver list
            reply->receivers.push_back(message->sender);
            break;
        }
    }

    if(reply->msg_type != MessageType::NO_ACTION){
        net.sendto(reply);
    }
    //delete(message);
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
