//
//  proposer.h
//  ChatService
//
//  Created by Pranav Ramarao on 3/5/17.
//  Copyright © 2017 Pranav Ramarao. All rights reserved.
//

#ifndef proposer_h
#define proposer_h

#include <stdio.h>
#include <string>
#include <map>
#include "message.h"

class Proposer{
private:
    std::map<int, int> count;
    
    int tot_replicas;
    int proposal_number;
    
    /* The value to be proposed */
    std::string to_propose;

public:
    Message start_prepare(int proposal_number);
    
    Message prepare_accept(int n_a, std::string value);
    
    Message prepare_reject(int n_p);
    
    Message propose_accept(int n);
    
    Message propose_reject(int n_p);
};
#endif /* proposer_h */
