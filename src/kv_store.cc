
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
    switch (message->msg_type) {
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
    reply->receivers.push_back(get_msg->sender);
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
    reply->value = learner_->get_latest_value(msg->key);
    return reply;
}

Message* KV_Store::handle_put_msg(Message* msg) {
    Message* msg = new Message();
    if (!is_primary(msg)) {
        msg->msg_type = MessageType::NO_ACTION;
        return msg;
    }
    msg->msg_type = MessageType::START_PREPARE;
    msg->view_num = msg->view_num;
    string true_value = msg->key
        + "#" + msg->value
        + "#" + to_string(put_msg->sender.port_)
        + "#" + msg->sender.host_
        + "#" + to_string(msg->seq_num);
    msg->value = true_value;
    msg->sender = env_->server_;
    env_->convert_msg_to_broadcast(msg);
    return msg;
}
