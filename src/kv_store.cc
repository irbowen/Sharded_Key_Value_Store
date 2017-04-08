
#include "../headers/kv_store.h"

void KV_Store::init(Learner* learner, int port, std::string host, std::string config_filename) {
    learner_ = learner;
    port_ = port;
    host_ = host;

    string h, p, rep_id;
    ifstream config_fs(config_filename);
    while (config_fs >> h >> p >> rep_id) {
        replicas_.push_back(node(stoi(p), h));
    }
}

Message* KV_Store::handle_get_msg(Message* get_msg) {
    // Scan log and get data
}

Message* KV_Store::handle_put_msg(Message* put_msg) {
    // We want to run paxos on this key/value proposal
    Message msg;
    msg.msg_type = MessageType::START_PREPARE;

    string true_value = put_msg->value
        + "#" + to_string(port_)
        + "#" + host_
        + "#" + to_string(0); // TODO: Remove?

    msg.key = put_msg->key;
    msg.value = true_value;

    msg.sender = node(port_, host_);
    for (auto& r : replicas_) {
        msg.receivers.push_back(r);
    }

    int cur_view_num = 0;
    while (true) {
        msg.view_num = cur_view_num;
        net.sendto(&msg);
        Message *reply = net.recv_from_with_timeout();
        if (reply != nullptr && reply->msg_type == MessageType::PROPOSAL_LEARNT) {
            COUT << "Key, Value pair: " << msg.key << " " << msg.value << endl;
            delete(reply);
        }
        cur_view_num += 1;
    }

    // Paxos run done
    // TODO respond to the client telling them that this key has beena added to the log

}
