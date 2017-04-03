
#include <iostream>

#include "../headers/learner.h"

using namespace std;

Message* Learner::handle_learn_msg(int in_view, int seq_num, string value) {
    lock_guard<mutex> lock(m);
    Message* reply = new Message;

    score_map[seq_num].tally += 1;
    score_map[seq_num].value = value;
    // If this is the message that made us equal to the qurom
    if (score_map[seq_num].tally == quorum) {
        // We should broadcast a proposal learned msg to everyone
        reply->msg_type = MessageType::PROPOSAL_LEARNT;
        reply->seq_num = seq_num;
        reply->value = value;
        reply->view_num = in_view;
        // commit message to local chat log
        if (static_cast<int>(log.size()) <= seq_num) {
            log.resize(seq_num + 1);
        }
        log.at(seq_num) = value;
        print_log();
    }
    return reply;
}

Message* Learner::answer_status_request(int seq_num) {
    lock_guard<mutex> lock(m);

    Message* reply = new Message;
    reply->msg_type = MessageType::STATUS;
    reply->seq_num = seq_num;

    if (log.size() <= seq_num) {
        reply->status = UNKNOWN;
    }
    else if (log.at(seq_num) != "") {
        reply->status = KNOWN;
        reply->value = log.at(seq_num);
    }
    return reply;
}

void Learner::print_log() {
    string log_filename = "log_" + to_string(id) + ".txt";
    ostringstream oss;
    for (auto& msg : log) {
        if(msg == "")
            msg = NO_OP;
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
    return static_cast<int>(log.size());
}

int Learner::get_seqnum_with_skip() {
    log.push_back("");
    return get_seqnum();
}
