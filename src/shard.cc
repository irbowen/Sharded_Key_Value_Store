
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
        Message* next_msg = msg_queue_.front();
        msg_queue_.pop();
        Message* reply = new Message();
        switch (next_msg->msg_type) {
            case MessageType::NO_ACTION:
                break;
            case MessageType::GET: {
                reply = handle_get(next_msg->key);
                break;
            }
            case MessageType::PUT: {
                reply = handle_put(next_msg->key, next_msg->value);
                break;
            }
            case MessageType::DELETE: {
                reply = handle_delete(next_msg->key);
                break;
            }
            break;
        }
    }
}

void Shard::register_msg(Message* message) {
    lock_guard<mutex> lock(m);
    msg_queue_.push(message);
    cv.notify_one();
}

Message* Shard::handle_get(string key) {
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
            // Send the same message we just got back to the client
            return reply;
        }
        cur_view_num_ += 1;
    }
}

Message* Shard::handle_put(string key, string value) {
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
            cout << "THIS IS A PUT ACK{{" << reply->get_key()
                << " " << reply->get_value()  << "}}" << endl;
            client_seq_num_++;
            // Send the same message we just got back to the client
            return reply;
        }
        cur_view_num_ += 1;
    }
}

Message* Shard::handle_delete(string key) {
    return handle_put(key, "ERROR: Key not found");
}

