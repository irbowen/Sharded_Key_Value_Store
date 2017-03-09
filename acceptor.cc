
#include "acceptor.h"

Message* Acceptor::accept_prepare_msg(int in_view) {
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

Message* Acceptor::accept_propose_msg(int in_view, std::string new_value, int seq_num) {
    Message *msg = new Message;
    if (in_view < cur_view) {
        msg->msg_type = MessageType::PROPOSE_REJECT;
    } else {
        cur_view = in_view;
        value = new_value;
        if (static_cast<int>(accepted_values.size()) <= seq_num) {
            accepted_values.resize(seq_num + 1);
        }
        // Accepted values now contains, at slot seq_nu, the value proposed and
        // the viewnumber that proposed it.
        accepted_values.at(seq_num) = view_val(cur_view, value);
        // Broadcast the fact that this acceptor has accepted to all learners
        msg->msg_type = MessageType::BRDCST_LEARNERS;
        msg->value = value;
        msg->seq_num = seq_num;
    }
    msg->view_num = cur_view;
    return msg;
}

void Acceptor::init(size_t _num_replicas, int _id) {
    num_replicas = _num_replicas;
    id = _id;
}
