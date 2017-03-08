
#include "acceptor.h"

Message* Acceptor::prepare(int in_view) {
    Message *msg = new Message;
    if (in_view < cur_view) {
        msg->msg_type = MessageType::PREPARE_REJECT;
    } else {
        msg->msg_type = MessageType::PREPARE_ACCEPT;
        msg->value = value;
    }
    msg->view_num = cur_view;
    return msg;
}

Message* Acceptor::propose(int in_view, std::string new_value){
    Message *msg = new Message;
    if (in_view < cur_view) {
        msg->msg_type = MessageType::PROPOSE_REJECT;
    } else {
        cur_view = in_view;
        value = new_value;
        msg->msg_type = MessageType::BRDCST_LEARNERS;
        msg->value = new_value;
    }
    msg->view_num = cur_view;
    return msg;
}

void Acceptor::init(size_t _num_replicas, int _id) {
    num_replicas = _num_replicas;
    id = _id;
}
