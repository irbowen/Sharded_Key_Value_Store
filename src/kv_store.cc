
#include "../headers/kv_store.h"

KV_Store::KV_Store(Environment* env, Learner* learner) {
    env_ = env;
    learner_ = learner;
}

bool KV_Store::is_primary(Message* msg) {
    return msg->view_num % env_->num_replicas_ == env_->replica_id_;
}

Message* KV_Store::handle_msg(Message* msg) {
    Message* reply = new Message();
    // Debug output
    if (env_->is_debug() && is_primary(msg)) { cout << "KV-Store recv'ed: " << msg->serialize() << endl; }
    switch (msg->msg_type) {
        case MessageType::GET:
            return handle_get_msg(msg);
        case MessageType::PUT:
            return handle_put_msg(msg);
        case MessageType::GET_KEYS:
            return handle_get_all_keys(msg);
    }
    // Should never get to this point
    return reply;
}

Message* KV_Store::handle_get_all_keys(Message* msg){
    Message* reply = new Message();
    if (!is_primary(msg)) {
        reply->msg_type = MessageType::NO_ACTION;
        return reply;
    }
    reply->msg_type = MessageType::PROPOSAL_LEARNT;
    reply->receivers.push_back(msg->sender);
    reply->all_keys = learner_->get_all_keys();
    return reply;
}

Message* KV_Store::handle_get_msg(Message* msg) {
    Message* reply = new Message();
    if (!is_primary(msg)) {
        reply->msg_type = MessageType::NO_ACTION;
        return reply;
    }
    reply->msg_type = MessageType::PROPOSAL_LEARNT;
    reply->receivers.push_back(msg->sender);
    reply->key = msg->key;
    reply->value = learner_->get_latest_value(msg->key, msg->column);
    return reply;
}

Message* KV_Store::handle_put_msg(Message* msg) {
    Message* reply = new Message();
    if (!is_primary(msg)) {
        reply->msg_type = MessageType::NO_ACTION;
        return reply;
    }
    reply->msg_type = MessageType::START_PREPARE;
    reply->view_num = msg->view_num;
    string true_value = msg->key
        + "#" + msg->column
        + "#" + msg->value
        + "#" + to_string(msg->sender.port_)
        + "#" + msg->sender.host_
        + "#" + to_string(msg->seq_num);
    reply->value = true_value;
    reply->sender = env_->server_;
    env_->convert_msg_to_broadcast(reply);
    if (env_->is_debug()) { cout << "KV Store has crated this msg: " << reply->serialize() << endl; }

    return reply;
}
