
#include "proposer.h"

void Proposer::init(vector<node> _replicas, int _id) {
    replicas = _replicas;
    quorum = (1 + _replicas.size()) >> 1;
    COUT << "zQuorum is: " << quorum << endl;
    id = _id;
}

Message* Proposer::handle_start_prepare(int view_num) {
    Message *msg = new Message;
    msg->msg_type = MessageType::PREPARE;
    msg->view_num = view_num;
    return msg;
}

bool Proposer::reached_quroum(int view_num) {
    return count[view_num] >= quorum;
}

// TODO check view vs proposal
// Note: copying vector by value to avoid delete invalidation
Message* Proposer::handle_prepare_accept(std::vector<view_val> acceptor_state, int view_num, std::string value, int seq_num) {
    Message *msg = new Message;

    if(is_new_primary)
        count[view_num] += 1;

    if (count[view_num] >= quorum && is_new_primary) {
        // i am the new primary, i have reached a quorum, i have f+1 acceptor states
        // i can begin the fix process


        // need to fix things
        is_new_primary = false;
    }

    if(count[view_num] >= quorum){
        // regular case where proposer proposes the original value
        msg->msg_type = MessageType::PROPOSE;
        // bug fixed : view num can never be -1 (we did this earlier since we had n_a)
        if (value == "") {
            // Propose the original value
            msg->value = to_propose;
            msg->view_num = view_num;
        } else {
            // Propose the already accepted value
            msg->value = value;
            msg->view_num = view_num;
        }
        // TODO : Need to add sequence number to this message (Important) - Pranav
        // if quorum is not reached, the message type default is NO_ACTION
        msg->seq_num = seq_num;
    }
    return msg;
}

Message* Proposer::handle_prepare_reject(int view_num) {
    // go back to prepare phase
    return handle_start_prepare(view_num + 1);
}

Message* Proposer::handle_propose_accept(int n) {
    // nothing to do for now in this scenario
    Message *msg = new Message;
    return msg;
}

Message* Proposer::handle_propose_reject(int view_num) {
    // if the proposal gets rejected, we are back to the prepare phase
    return handle_start_prepare(view_num + 1);
}
