//
//  proposer.cc
//  ChatService
//
//  Created by Pranav Ramarao on 3/5/17.
//  Copyright © 2017 Pranav Ramarao. All rights reserved.
//

#include "proposer.h"

Message Proposer::prepare_accept(int n_a, std::string value){
    Message msg;

    count[proposal_number] += 1;
    int quorum = (1 + tot_replicas) >> 2;
    if(count[proposal_number] >= quorum){
        msg.msg_type = MessageType::PROPOSE;

        if(n_a == -1){
            // Propose the original value
            msg.value = to_propose;
        } else{
            // Propose the already accepted value
            msg.value = value;
        }
        return msg;
    }

    // if quorum is not reached, the message type default is NO_ACTION
    return msg;
}
Message Proposer::start_prepare(int proposal_number){
    Message msg;
    msg.msg_type = MessageType::PREPARE;
    msg.prop_number = proposal_number;
    return msg;
}

Message Proposer::prepare_reject(int n_p){
    // go back to prepare phase
    return start_prepare(n_p + 1);
}

Message Proposer::propose_accept(int n){
    // nothing to do for now in this scenario
    Message msg;
    return msg;
}

Message Proposer::propose_reject(int n_p){
    // if the proposal gets rejected, we are back to the prepare phase
    return start_prepare(n_p + 1);
}
