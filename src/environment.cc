
#include "../headers/environment.h"

/* Setting up the replica with the provided port and host */
environment::environment(int port, string host, int id, string config_file,
    network* net) : server_(port, host), replica_id_(id)
{
    string h, p, rep_id;
    ifstream config_fs(config_file);
    while (config_fs >> h >> p >> rep_id) {
        replicas_.push_back(node(stoi(p), h));
    }
    num_replicas_ = replicas_.size();
    net_ = net;
}

/* Remove all current recipients, and add all replicas in the
   environment to the mesages receiver list */
void environment::convert_msg_to_broadcast(Message* msg) {
    msg->receivers.clear();
    for (auto& r: replicas_) {
        msg->receivers.push_back(r);
    }
}
