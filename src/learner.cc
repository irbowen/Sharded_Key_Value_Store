
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include "../headers/learner.h"

using namespace std;

Message* Learner::handle_learn_msg(int in_view, int seq_num, string value, Message* msg) {
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
            object_log.resize(seq_num + 1);
        }
        log.at(seq_num) = value;
        object_log.at(seq_num) = Object(msg->get_key(), msg->get_value());
        print_log();
    }
    return reply;
}

string Learner::get_latest_value(string key) {
    lock_guard<mutex> lock(m);
    auto res = find_if(object_log.rbegin(), object_log.rend(),
        [key](auto& obj) {
            return obj.key == key;
    });
    if (res == object_log.rend() || res->value == "DELETED_KEY") {
        return "ERROR - Key not found";
    }
    return res->value;

}
vector<string> Learner::get_all_keys(){
    lock_guard<mutex> lock(m);
    unordered_set<string> seen_keys;
    vector<string> _all_keys;
    for(int i = object_log.size() - 1; i >=0; i--){
        string key = object_log[i].key;
        if(key != "DELETED_KEY" &&  seen_keys.count(key) == 0){
            _all_keys.push_back(key);
            seen_keys.insert(key);
        }
    }
    return _all_keys;
}
void Learner::print_log() {
    string log_filename = "log_" + to_string(port) + "_" + to_string(id) +  + ".txt";
    ostringstream oss;
    for (auto& msg : object_log) {
//        if(msg. == "")
//            msg = NO_OP;
        oss << msg.key << ":" << msg.value << ", ";
    }
    ofstream outfile;
    outfile.open(log_filename, ios_base::app);
    outfile << oss.str() << endl << endl;
}


void Learner::init(size_t replica_count, size_t _id, int port) {
    this->port = port;
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
