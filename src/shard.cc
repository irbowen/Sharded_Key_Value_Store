
#include "../headers/shard.h"

using namespace std;

Shard::Shard(int port, std::string host, std::string config_filename)
    : port_(port), host_(host), config_filename_(config_filename), net_(port, host)
{
    string h, p, rep_id;
    ifstream config_fs(config_filename);
    cout << "[Master_Shard_Object] (" << host << ":" << port << ") is online with these replicas: ";
    while (config_fs >> h >> p >> rep_id) {
        replicas_.push_back(node(stoi(p), h));
        cout << " " << host << ":" << p << ", ";
    }
    cout << endl;
}

string Shard::get_port_host(){
    return to_string(port_) + host_;
}

void Shard::run() {
    while (true) {
        unique_lock<mutex> lock(m);
        while (msg_queue_.empty()) {
            cv.wait(lock);
        }
        Message* next_msg = msg_queue_.front();
        msg_queue_.pop();
        lock.unlock();
        Message* reply = new Message();
        switch (next_msg->msg_type) {
            case MessageType::NO_ACTION:
                break;
            case MessageType::GET: {
                reply = handle_get(next_msg->key, next_msg->column, next_msg->sender);
                break;
            }
            case MessageType::PUT: {
                reply = handle_put(next_msg->key, next_msg->column, next_msg->value, next_msg->sender);
                break;
            }
            case MessageType::DELETE: {
                reply = handle_delete(next_msg->key, next_msg->column, next_msg->sender);
                break;
            }
            case MessageType::GET_KEYS: {
                reply = handle_get_all_keys(next_msg->sender);
                break;
            }
            break;
        }
        if (reply != nullptr && reply->msg_type != MessageType::NO_ACTION) {
            reply->sender = node(port_, host_);
            //cout << "[Master_Shard_Object] - Sending reply: " << reply->serialize() << endl;
            net_.sendto(reply);
        }
        delete(next_msg);
        delete(reply);
    }
}

void Shard::register_msg(Message* message) {
    lock_guard<mutex> lock(m);
    msg_queue_.push(message);
    cv.notify_one();
}

Message* Shard::handle_get(string key, string column, node sender) {
    Message msg;
    msg.seq_num = client_seq_num_;
    msg.msg_type = MessageType::GET;
    msg.key = key;
    msg.column = column;
    msg.sender = node(port_, host_);
    for(auto& r : replicas_){
        msg.receivers.push_back(r);
    }
    net_.set_start_timeout_factor(4);
    while (true) {
        // run paxos for getting the message, so that the new primary if any is
        // up to date with the latest values that took place so far
        // TODO: think abou this some more
        handle_put("READ_KEY", key, column, sender);
        msg.view_num = cur_view_num_;
        net_.sendto(&msg);
        Message *reply = net_.recv_from_with_timeout();
        if (reply != nullptr && reply->msg_type == MessageType::PROPOSAL_LEARNT) {
            string val = reply->value;
            //cout << "[Master_Shard_Object] - Got value: " << val << endl;
            client_seq_num_++;
            reply->msg_type = MessageType::MASTER_ACK;
            reply->receivers.clear();
            reply->receivers.push_back(sender);
            return reply;
        }
        cur_view_num_ += 1;
        cout << "View Num Changed to: " << cur_view_num_ << endl;

    }
}

Message* Shard::handle_put(string key, string column, string value, node sender) {
    Message msg;
    msg.seq_num = client_seq_num_;
    msg.msg_type = MessageType::PUT;
    msg.key = key;
    msg.column = column;
    msg.value = value;
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
            //cout << "[Master_Shard_Object] - Put ack {{" << reply->get_key()
            //    << " " << reply->get_value()  << "}}" << endl;
            client_seq_num_++;
            reply->msg_type = MessageType::MASTER_ACK;
            reply->receivers.clear();
            reply->receivers.push_back(sender);
            return reply;
        }
        cur_view_num_ += 1;
        cout << "View Num Changed to: " << cur_view_num_ << endl;
    }
}

Message* Shard::handle_get_all_keys(node sender){
    Message msg;
    msg.seq_num = client_seq_num_;
    msg.msg_type = MessageType::GET_KEYS;
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
            //cout << "[Master_Shard_Object] - Get all keys - ack {{" << "}}" << endl;
            client_seq_num_++;
            reply->msg_type = MessageType::MASTER_ACK;
            reply->receivers.clear();
            reply->receivers.push_back(sender);
            return reply;
        }
        cur_view_num_ += 1;
    }
}

Message* Shard::handle_delete(string key, string column, node sender) {
    return handle_put(key, column, "DELETED_KEY", sender);
}

