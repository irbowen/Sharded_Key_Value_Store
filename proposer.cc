
#include "proposer.h"

void Proposer::init(vector<node> _replicas, int _id) {
    replicas = _replicas;
    quorum = (1 + _replicas.size()) >> 1;
    id = _id;
}

Message* Proposer::start_prepare(int view_num, int seq_num) {
    Message *msg = new Message;
    msg->msg_type = MessageType::PREPARE;
    msg->view_num = view_num;
    msg->seq_num = seq_num;
    return msg;
}

Message* Proposer::prepare_accept(int view_num, std::string value) {
    Message *msg = new Message;
    count[proposal_number] += 1;
    if (count[proposal_number] >= quorum) {
        msg->msg_type = MessageType::PROPOSE;
        if (view_num == -1) {
            // Propose the original value
            msg->value = to_propose;
            msg->view_num = proposal_number;
        } else {
            // Propose the already accepted value
            msg->value = value;
            msg->view_num = proposal_number;
        }
    }
    // if quorum is not reached, the message type default is NO_ACTION
    return msg;
}

Message* Proposer::prepare_reject(int view_num, int seq_num) {
    // go back to prepare phase
    return start_prepare(view_num + 1, seq_num);
}

Message* Proposer::propose_accept(int n) {
    // nothing to do for now in this scenario
    Message *msg = new Message;
    return msg;
}

Message* Proposer::propose_reject(int view_num, int seq_num) {
    // if the proposal gets rejected, we are back to the prepare phase
    return start_prepare(view_num + 1, seq_num);
}

