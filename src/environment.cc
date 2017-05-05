
#include <string>
#include <fstream>

#include "../headers/environment.h"

/* Setting up the replica with the provided port and host */
environment::environment(int port, string host, int id, string config_file,
    network* net) : server_(port, host), id_(id)
{
    string h, p, rep_id;
    ifstream config_fs(config_file);
    while (config_fs >> h >> p >> rep_id) {
        replicas_.push_back(node(stoi(p), h));
    }
    num_replicas_ = replicas_.size();
    net_ = net;
}
