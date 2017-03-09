
#include "acceptor.h"

Message* Acceptor::prepare(int in_view) {
    Message *msg = new Message;
    if (in_view < cur_view) {
        msg->msg_type = MessageType::PREPARE_REJECT;
    } else {
        cur_view = in_view;
        msg->msg_type = MessageType::PREPARE_ACCEPT;
    }
    msg->view_num = cur_view;
    msg->acceptor_state = accepted_values;
    return msg;
}

Message* Acceptor::propose(int in_view, std::string new_value, int seq_num) {
    Message *msg = new Message;
    if (in_view < cur_view) {
        msg->msg_type = MessageType::PROPOSE_REJECT;
    } else {
        if (static_cast<int>(accepted_values.size()) <= seq_num) {
            accepted_values.resize(seq_num + 1);
        }
        // Accepted values now contains, at slot seq_nu, the value proposed and
        // the viewnumber that proposed it.
        accepted_values.at(seq_num) = view_val(in_view, new_value);
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
