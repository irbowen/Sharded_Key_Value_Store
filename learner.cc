
#include <iostream>

#include "learner.h"

using namespace std;

Message* Learner::update_vote(int n_a, string value){
    Message *reply = new Message;
    
    score_map[n_a].tally += 1;
    score_map[n_a].value = value;
    if (score_map[n_a].tally >= quorum){
        reply->msg_type = MessageType::PROPOSE_ACCEPT;
        // commit message to local chat log
        // set seq_num -> seq_num + 1
        if (static_cast<int>(chat_log.size()) <= n_a) {
            chat_log.resize(n_a + 1);
        }
        chat_log.at(n_a) = value;
        print_log();
    }
    return reply;
}

void Learner::print_log() {
    string log_filename = "log_" + to_string(id) + ".txt";
    ostringstream oss;
    for (auto& msg : chat_log) {
        oss << msg << ", ";
    }
    ofstream outfile;
    outfile.open(log_filename, ios_base::app);
    outfile << oss.str() << endl;
}


void Learner::init(size_t replica_count, int _id) {
    quorum = (1 + replica_count) >> 1;
    id = _id;
}

