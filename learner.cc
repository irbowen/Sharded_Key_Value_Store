//
//  learner.cc
//  ChatService
//
//  Created by Pranav Ramarao on 3/5/17.
//  Copyright © 2017 Pranav Ramarao. All rights reserved.
//

#include "learner.h"

Message Learner::update_vote(int n_a, std::string value){
    Message reply;

    score_map[n_a].tally += 1;
    score_map[n_a].value = value;

    int quorum;
    if(score_map[n_a].tally >= quorum){
        reply.msg_type = MessageType::PROPOSE_ACCEPT;
        // commit message to local chat log
        // set seq_num -> seq_num + 1
    }
    return reply;
}
