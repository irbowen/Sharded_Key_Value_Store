
#include "../headers/kv_store.h"

KV_Store::KV_Store(Learner* l) {
    this->learner = l;
}

Message* KV_Store::handle_get_msg(Message* get_msg) {
    // Scan log and get data
}

Message* KV_Store::handle_put_msg(Message* put_msg) {
    // We want to run paxos on this key/value proposal
    Message msg;
    msg.msg_type = MessageType::START_PREPARE;
    msg.view_num = 0; //TODO

    msg.key = put_msg->key;
    string true_value = chat_message 
        + "#" + to_string(port)
        + "#" + host
        + "#" + to_string(client_seq_num);
    msg.value = true_value;
    msg.sender = node(port, host);
    for(auto& r : replicas){
        msg.receivers.push_back(r);
    }
}
