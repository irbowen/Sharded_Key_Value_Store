//
//  learner.h
//  ChatService
//
//  Created by Pranav Ramarao on 3/5/17.
//  Copyright © 2017 Pranav Ramarao. All rights reserved.
//

#ifndef learner_h
#define learner_h

#include <stdio.h>
#include <string>
#include "message.h"
#include <map>

struct Score{
    unsigned int tally;
    std::string value;
};

class Learner{
private:
    std::map<int, Score> score_map;
    
public:
    Message update_vote(int n_a, std::string value);
};

#endif /* learner_h */
