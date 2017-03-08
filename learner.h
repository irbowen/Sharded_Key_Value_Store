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

struct Score {
  size_t tally;
  std::string value;
};

class Learner {
  private:
    size_t quorum;
    std::map<int, Score> score_map;
    std::vector<std::string> chat_log;

  public:
    Learner();
    void init(int replica_count);
    Message update_vote(int n_a, std::string value);
    void print_log();
};

#endif /* learner_h */
