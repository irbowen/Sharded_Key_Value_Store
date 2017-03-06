//
//  acceptor.cc
//  ChatService
//
//  Created by Pranav Ramarao on 3/5/17.
//  Copyright © 2017 Pranav Ramarao. All rights reserved.
//

#include "acceptor.h"

Message Acceptor::prepare(int n){
    Message msg;
    if(n < n_p){
        msg.msg_type = MessageType::PREPARE_REJECT;
        msg.n_p = n_p;
    } else{
        msg.msg_type = MessageType::PREPARE_ACCEPT;
        msg.n_a = n_a;
        msg.value = value;
    }
    return msg;
}

Message Acceptor::propose(int n, std::string new_value){
    Message msg;
    if(n < n_p){
        msg.msg_type = MessageType::PROPOSE_REJECT;
        msg.n_p = n_p;
    } else{
        n_a = n;
        value = new_value;
        msg.msg_type = MessageType::PROPOSE_ACCEPT;
        msg.n = n;
    }
    return msg;
}
