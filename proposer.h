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

class Proposer{
public:
    void prepare_accept(int n_a, std::string value);
    
    void prepare_reject(int n_p);
    
    void propose_accept(int n);
    
    void propose_reject(int n_p);
};
#endif /* proposer_h */
