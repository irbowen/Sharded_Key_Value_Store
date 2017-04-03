
#include "../headers/proposer.h"

using namespace std;

void Proposer::init(vector<node> _replicas, int _id, network* net, vector<int> holes) {
    replicas = _replicas;
    seq_holes = holes;
    quorum = (1 + _replicas.size()) >> 1;
    //COUT << "zQuorum is: " << quorum << endl;
    id = _id;
    this->net = net;
}

Message* Proposer::handle_start_prepare(int view_num) {
    Message *msg = new Message;
    msg->msg_type = MessageType::PREPARE;
    msg->view_num = view_num;
    return msg;
}

Message* Proposer::handle_prepare_accept_fast(std::vector<view_val> acceptor_state, int view_num, std::string value, int seq_num) {
    Message *msg = new Message;

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
    return msg;
}

// TODO check view vs proposal
// Note: copying vector by value to avoid delete invalidation
Message* Proposer::handle_prepare_accept(std::vector<view_val> acceptor_state, int view_num, std::string value, int seq_num) {
    Message *msg = new Message;

    if(is_new_primary){
        all_acceptors_state.push_back(acceptor_state);
    }
    count[view_num] += 1;
    if (count[view_num] == quorum && is_new_primary) {
        // i am the new primary, i have reached a quorum, i have f+1 acceptor states
        // i can begin the fix process
        
        // find longest acceptor state
        auto max_length_acceptor = std::max_element(all_acceptors_state.begin(), all_acceptors_state.end(),
                                                    []( const std::vector<view_val> &a, const std::vector<view_val> &b )
                                                    {
                                                        return a.size() < b.size();
                                                    });
        // for each column
        for(int j = 0; j < max_length_acceptor->size(); j++){
            // for each item in that column
            std::map<int, int> quorum_count;
            bool quorum_reached = false;
            
            // Note: best_view_val has -1 view num and no_op value by default
            // if it doesn't get updated in the below loop, we can decide accordingly
            view_val best_view_val;
            for(int i = 0; i < all_acceptors_state.size(); i++){
                auto item = all_acceptors_state[i][j];
                quorum_count[item.view_num] += 1;
                if(quorum_count[item.view_num] >= quorum){
                    quorum_reached = true;
                }
                if(item.view_num > best_view_val.view_num){
                    best_view_val = item;
                }
            }
            if(!quorum_reached){
                Message *reply = new Message;
                reply->view_num = view_num; // view_num of the new primary
                reply->value = best_view_val.value; // will automatically be no_op if that is the best
                reply->seq_num = j; // jth column becomes the sequence number
                reply->msg_type = MessageType::PROPOSE;
                
                //broadcast message
                for (auto& r : replicas) {
                    reply->receivers.push_back(r);
                }
                reply->sender = replicas[id];   // quick hack to get proposer's info
                net->sendto(reply);
            }
            // if quorum is reached, do nothing for this column
        }
        // update sequence number to max_length
        seq_num = static_cast<int>(max_length_acceptor->size());
        
        while(is_seq_hole(seq_num)){
            cout << "Seq num " << seq_num << " is a hole. skipping to next seq num\n";
            seq_num += 1;
        }
        
        // update seq_num if required based on acceptor state
        // seq_num = new_seq_num;
        is_new_primary = false;
        all_acceptors_state.clear();
    }
    
    if(count[view_num] == quorum){
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

bool Proposer::reached_quroum(int view_num) {
    return count[view_num] >= quorum;
}

bool Proposer::is_seq_hole(int seq){
    return find(seq_holes.begin(), seq_holes.end(), seq) != seq_holes.end();
}

