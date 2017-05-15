
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
    if (env_->is_debug()) { cout << "Learner got msg " << msg->serialize() << endl; }
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
        object_log.at(seq_num) = Object(msg->get_key(), msg->get_column(), msg->get_value());
        print_log();
    }
    return reply;
}

string Learner::get_latest_value(string key, string column) {
    lock_guard<mutex> lock(m);
    auto res = find_if(object_log.rbegin(), object_log.rend(),
        [key, column](auto& obj) {
            return obj.key_ == key && obj.column_ == column;
    });
    if (res == object_log.rend() || res->value_ == "DELETED_KEY") {
        return "ERROR - Key not found";
    }
    return res->value_;

}

vector<string> Learner::get_all_keys(){
    lock_guard<mutex> lock(m);
    unordered_set<string> seen_keys;
    vector<Object> recent_obs;
    for (auto it = object_log.rbegin(); it < object_log.rend(); it++) {
        string key = it->key_;
        if (seen_keys.count(key) == 0) {
            recent_obs.push_back(*it);
            seen_keys.insert(key);
        }
    }
    vector<string> return_keys;
    for (auto& obj : recent_obs) {
        if (obj.value_ != "DELETED_KEY") {
            return_keys.push_back(obj.key_);
        }
    }
    cout << "Moving keys\n";
    for (auto& k : return_keys) {
        cout << "Key: " << k << ", ";
    }
    cout << endl;
    return return_keys;;
}
void Learner::print_log() {
    string log_filename = "log_" + env_->get_host_port_id_string() + ".txt";
    ostringstream oss;
    for (auto& obj : object_log) {
        if (obj.key_ == "READ_KEY") {
            continue;
        }
        oss << obj.key_ << ":" << obj.column_ << ":" << obj.value_ << ", ";
    }
    ofstream outfile;
    outfile.open(log_filename, ios_base::app);
    outfile << oss.str() << endl << endl;
}

void Learner::init(Environment* env) {
    env_ = env;
    quorum = (env_->num_replicas_ + 1) >> 1;
    if (env_->is_debug()) { cout << "Quorum: " << quorum << endl; }
}

int Learner::get_seqnum() {
    return static_cast<int>(log.size());
}

int Learner::get_seqnum_with_skip() {
    log.push_back("");
    return get_seqnum();
}
