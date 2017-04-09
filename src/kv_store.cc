
#include "../headers/kv_store.h"

KV_Store::KV_Store(int replica_id, int port, std::string host, std::string config_filename)
    : replica_id_(replica_id), port_(port), host_(host), net_(port, host)
{
    string h, p, rep_id;
    ifstream config_fs(config_filename);
    while (config_fs >> h >> p >> rep_id) {
        replicas_.push_back(node(stoi(p), h));
    }
}

void KV_Store::init(Learner* learner) {
    learner_ = learner;
}

void KV_Store::kv_recv() {
    while (true) {
        Message* msg = net_.recv_from();
        handle_kv_msg(msg);
    }
}

void KV_Store::handle_kv_msg(Message* message) {
    Message* reply;
    COUT << "Msg in handle_kv_msg: " << message->serialize() << endl;
    switch (message->msg_type) {
        case MessageType::PUT: {
            reply = handle_put_msg(message);
            break;
        }
        case MessageType::GET: {
            reply = handle_put_msg(message);
            break;
        }
    }
    if (reply != nullptr && reply->msg_type != MessageType::NO_ACTION) {
        reply->sender.host = host_;
        reply->sender.port = port_;
        net_.sendto(reply);
    }
    delete(message);
    delete(reply);
}


Message* KV_Store::handle_get_msg(Message* get_msg) {
    Message* ack_msg;
    ack_msg->msg_type = MessageType::GET_ACK;
    ack_msg->receivers.push_back(get_msg->sender);
    ack_msg->key = get_msg->key;
    ack_msg->value = learner_->get_latest_value(get_msg->key).value_or("err");
    return ack_msg;
}

Message* KV_Store::handle_put_msg(Message* put_msg) {

    if (put_msg->view_num % replicas_.size() != replica_id_) {
        Message* msg;
        msg->msg_type = MessageType::NO_ACTION;
        return msg;
    }
    Message msg;
    msg.msg_type = MessageType::START_PREPARE;

    string true_value = put_msg->value
        + "#" + to_string(port_)
        + "#" + host_
        + "#" + to_string(0); // TODO: Remove?

    msg.key = put_msg->key;
    msg.value = true_value;

    msg.sender = node(port_, host_);
    for (auto& r : replicas_) {
        msg.receivers.push_back(r);
    }

    int cur_view_num = 0;
    while (true) {
        msg.view_num = cur_view_num;
        net_.sendto(&msg);
        Message *reply = net_.recv_from_with_timeout();
        if (reply != nullptr && reply->msg_type == MessageType::PROPOSAL_LEARNT) {
            COUT << "Key, Value pair: " << msg.key << " " << msg.value << endl;
            delete(reply);
            break;
        }
        cur_view_num += 1;
    }

    Message* ack_msg;
    ack_msg->msg_type = MessageType::PUT_ACK;
    ack_msg->receivers.push_back(put_msg->sender);
    return ack_msg;
}






