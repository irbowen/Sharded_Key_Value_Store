//
//  learner.cc
//  ChatService
//
//  Created by Pranav Ramarao on 3/5/17.
//  Copyright © 2017 Pranav Ramarao. All rights reserved.
//

#include <iostream>

#include "learner.h"

using namespace std;

Learner::Learner() {}

void Learner::init(int num_replicas) {
  // Off by one?
  cout << "Num replicas: " << num_replicas << endl;
  quorum = ((num_replicas - 1) / 2) + 1;
  cout << "Quroum: " << quorum << endl;
}

Message Learner::update_vote(int n_a, string value){
  Message reply;

  score_map[n_a].tally += 1;
  score_map[n_a].value = value;

  if (score_map[n_a].tally >= quorum) {
    reply.msg_type = MessageType::PROPOSE_ACCEPT;
    // commit message to local chat log
    // set seq_num -> seq_num + 1
    if (static_cast<int>(chat_log.size()) < n_a) {
      chat_log.resize(n_a + 1);
    }
    chat_log.at(n_a) = value;
    print_log();
  }
  return reply;
}

void Learner::print_log() {
  cout << "CHAT LOG: ";
  for (auto& msg : chat_log) {
    cout << msg << endl;
  }
}
