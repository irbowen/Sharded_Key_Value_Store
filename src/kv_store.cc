
#include "../headers/kv_store.h"

using namespace std;

KV_Store::KV_Store(int replica_id, int port, std::string host, vector<node> replicas)
    : replica_id_(replica_id), port_(port), host_(host), replicas_(replicas)
{}

void KV_Store::init(Learner* learner) {
    learner_ = learner;
}

Message* KV_Store::handle_get_msg(Message* get_msg) {
    if (get_msg->view_num % replicas_.size() != replica_id_) {
        Message* msg = new Message();
        msg->msg_type = MessageType::NO_ACTION;
        return msg;
    }
    Message* ack_msg = new Message();
    ack_msg->msg_type = MessageType::PROPOSAL_LEARNT;
    ack_msg->receivers.push_back(get_msg->sender);
    ack_msg->key = get_msg->key;
    ack_msg->value = learner_->get_latest_value(get_msg->key).value_or("err");
    return ack_msg;
}

Message* KV_Store::handle_put_msg(Message* put_msg) {
    Message* msg = new Message();
    if (put_msg->view_num % replicas_.size() != replica_id_) {
        msg->msg_type = MessageType::NO_ACTION;
        return msg;
    }
    msg->msg_type = MessageType::START_PREPARE;
    msg->view_num = put_msg->view_num;
    msg->key = put_msg->key;
    msg->value = put_msg->value;
    msg->sender = node(port_, host_);
    msg->client = put_msg->sender;
    for (auto& r : replicas_) {
        msg->receivers.push_back(r);
    }
    cout << "Got a put I could answer\n";
    cout << put_msg->serialize() << endl;
    cout << msg->serialize() << endl;
    return msg;
}

