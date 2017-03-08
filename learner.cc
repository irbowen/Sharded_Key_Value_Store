
#include <iostream>

#include "learner.h"

using namespace std;

Message* Learner::update_vote(int n_a, int seq_num, string value){
    lock_guard<mutex> lock(m);
    Message *reply = new Message;
    
    score_map[n_a].tally += 1;
    score_map[n_a].value = value;
    // If this is the message that made us equal to the qurom
    if (score_map[n_a].tally == quorum) {
        // We should broadcast a proposal learned msg to everyone
        reply->msg_type = MessageType::PROPOSAL_LEARNT;
        // commit message to local chat log
        // set seq_num -> seq_num + 1
        if (static_cast<int>(chat_log.size()) <= seq_num) {
            chat_log.resize(seq_num + 1);
        }
        chat_log.at(seq_num) = value;
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


void Learner::init(size_t replica_count, size_t _id) {
    quorum = (1 + replica_count) >> 1;
    id = _id;
}

int Learner::get_seqnum() {
    return static_cast<int>(chat_log.size());
}

int Learner::get_seqnum_with_skip() {
    chat_log.push_back("");
    return get_seqnum();    
}
