
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
replica::replica(environment* env, string _holes_file) : cur_view_num(-1)
{
    if (_holes_file != "") {
        string hole;
        ifstream holes_fs(_holes_file);
        while (holes_fs >> hole) {
            seq_holes.push_back(stoi(hole));
        }
    }
    auto id = env->id_;
    auto port = env->server_.port_;
    auto host = env->server_.host_;
    auto num_replicas = env->num_replicas_;
    acceptor.init(num_replicas, id);
    learner.init(num_replicas, id, port);
    proposer.init(replicas, id, env->net_, seq_holes);

    /* Set up the key value store object */
    kv_store_ = make_unique<KV_Store>(id, port, host, replicas);
    kv_store_->init(&learner);
    env_ = env;
}

void replica::recv() {
    while (true) {
        Message* msg = env_->net_->recv_from();
        handle_msg(msg);
    }
}

/* Start listening on the provided port and host */
void replica::start() {
    this->recv();
}

/* Adds all the replicas in the system to the receiver list */
void replica::make_broadcast(Message* ref) {
    for (auto& r: replicas) {
        ref->receivers.push_back(r);
    }
}

bool replica::is_primary(int view_num) {
    return cur_view_num % num_replicas == id && proposer.reached_quroum(view_num);
}

bool replica::is_previous_view(int view_num) {
    return seen_views.count(view_num) == 1;
}

/* Handle the given message, handing off to whichever class should
   be responsible for the given message */
void replica::handle_msg(Message* message) {
    Message* reply = new Message();
    if (message->is_paxos_msg()) {
        reply = kv_store_.handle_msg(message);
    }
    if (message->is_kv_msg()) {
        reply = paxos_.handle_msg(message);
    }
    if (reply != nullptr && reply->msg_type != MessageType::NO_ACTION) {
        reply->sender.host_ = env_->server_.host_;
        reply->sender.port_ = env_->server_.port_;
        env_->net_->sendto(reply);
    }
    delete(message);
    delete(reply);
}
