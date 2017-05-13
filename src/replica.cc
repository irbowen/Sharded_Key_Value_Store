
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>

#include "../headers/replica.h"
#include "../headers/message.h"

using namespace std;

/* Setting up the replica with the provided port and host */
Replica::Replica(Environment* env, string holes_file) {
    /* Set up the paxos object */
    paxos_ = make_unique<Paxos>(env, holes_file);
    /* Set up the key value store object */
    kv_store_ = make_unique<KV_Store>(env, paxos_->get_learner());
    env_ = env;
}

void Replica::start() {
    while (true) {
        Message* msg = env_->net_->recv_from();
        handle_msg(msg);
    }
}

/* Handle the given message, handing off to whichever class should
   be responsible for the given message */
void Replica::handle_msg(Message* msg) {
    Message* reply = new Message();
    if (env_->is_debug()) { cout << "Replica recv'ed: " << msg->serialize() << endl; }

    if (msg->is_paxos_msg()) {
        reply = paxos_->handle_msg(msg);
    }
    if (msg->is_kv_msg()) {
        reply = kv_store_->handle_msg(msg);
    }
    if (reply != nullptr && reply->msg_type != MessageType::NO_ACTION) {
        reply->sender.host_ = env_->server_.host_;
        reply->sender.port_ = env_->server_.port_;
        env_->net_->sendto(reply);
    }
    delete(msg);
    delete(reply);
}
