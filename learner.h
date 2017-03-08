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
#include <fstream>
#include "message.h"
#include <map>
#include <mutex>
#include <thread>

#define DIF 2

struct Score {
    size_t tally;
    std::string value;
};

class Learner {
private:
	std::mutex m;
    size_t quorum;
    size_t id;
    std::map<int, Score> score_map;
    std::vector<std::string> chat_log;
    
public:
    void init(size_t replica_count, size_t _id);
    Message* update_vote(int n_a, int seq_num, std::string value);
    void print_log();
    size_t get_seqnum();
    size_t get_seqnum_with_skip();
};

#endif /* learner_h */
