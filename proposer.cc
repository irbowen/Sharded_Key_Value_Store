
#include "proposer.h"

void Proposer::init(vector<node> _replicas, int _id) {
    replicas = _replicas;
    quorum = (1 + _replicas.size()) >> 1;
    id = _id;
}

Message* Proposer::start_prepare(int view_num) {
    Message *msg = new Message;
    msg->msg_type = MessageType::PREPARE;
    msg->view_num = view_num;
    return msg;
}

bool Proposer::reached_quroum(int view_num) {
  return count[view_num] >= quorum;
}

// TODO check view vs proposal
Message* Proposer::prepare_accept(int view_num, std::string value) {
    Message *msg = new Message;
    count[view_num] += 1;
    if (count[view_num] >= quorum) {
        msg->msg_type = MessageType::PROPOSE;
        if (view_num == -1) {
            // Propose the original value
            msg->value = to_propose;
            msg->view_num = view_num;
        } else {
            // Propose the already accepted value
            msg->value = value;
            msg->view_num = view_num;
        }
    }
    // if quorum is not reached, the message type default is NO_ACTION
    return msg;
}

Message* Proposer::prepare_reject(int view_num) {
    // go back to prepare phase
    return start_prepare(view_num + 1);
}

Message* Proposer::propose_accept(int n) {
    // nothing to do for now in this scenario
    Message *msg = new Message;
    return msg;
}

Message* Proposer::propose_reject(int view_num) {
    // if the proposal gets rejected, we are back to the prepare phase
    return start_prepare(view_num + 1);
}
