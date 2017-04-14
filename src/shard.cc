
#include "../headers/shard.h"

using namespace std;

Shard::Shard(int port, std::string host, std::string config_filename)
    : config_filename_(config_filename), net_(port, host)
{
    string h, p, rep_id;
    ifstream config_fs(config_filename);
    while (config_fs >> h >> p >> rep_id) {
        replicas_.push_back(node(stoi(p), h));
    }
}

void Shard::run() {
    unique_lock<mutex> lock(m);
    while (true) {
        cv.wait(lock);
        // Grab next thing from queue
        // Call put,get, and then return to client
    }
}

void Shard::register_msg(Message* message) {
    lock_guard<mutex> lock(m);
    msg_queue_.push(message);
    cv.notify_one();
}

string Shard::get(string key) {
    // TODO CHECK QUEUE
    Message msg;
    msg.seq_num = client_seq_num_;
    msg.msg_type = MessageType::GET;
    msg.key = key;
    msg.sender = node(port_, host_);
    for(auto& r : replicas_){
        msg.receivers.push_back(r);
    }
    net_.set_start_timeout_factor(4);
    while (true) {
        msg.view_num = cur_view_num_;
        net_.sendto(&msg);
        Message *reply = net_.recv_from_with_timeout();
        if (reply != nullptr && reply->msg_type == MessageType::PROPOSAL_LEARNT) {
            string val = reply->value;
            cout << "THIS IS A GET ACK{{" << val << "}}" << endl;
            client_seq_num_++;
            delete(reply);
            return val;
        }
        cur_view_num_ += 1;
    }
}

void Shard::put(string key, string value) {
    // TODO CHECK QUEUE
    Message msg;
    msg.seq_num = client_seq_num_;
    msg.msg_type = MessageType::PUT;
    msg.key = key;
    msg.value = value;
    msg.sender = node(port_, host_);
    for(auto& r : replicas_){
        msg.receivers.push_back(r);
    }
    net_.set_start_timeout_factor(4);
    while (true) {
        msg.view_num = cur_view_num_;
        net_.sendto(&msg);
        cout << "Sending msg: " << msg.serialize() << endl;
        Message *reply = net_.recv_from_with_timeout();
        if (reply != nullptr && reply->msg_type == MessageType::PROPOSAL_LEARNT 
                && reply->get_key() == key && reply->get_value() == value) {
            cout << "THIS IS A PUT ACK{{" << reply->get_key() << " " << reply->get_value()  << "}}" << endl;
            client_seq_num_++;
            delete(reply);
            return;
        }
        cur_view_num_ += 1;
    }
}

