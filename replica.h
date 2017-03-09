
#ifndef replica_h
#define replica_h

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <condition_variable>
#include <thread>
#include <vector>

#include "acceptor.h"
#include "learner.h"
#include "network.h"
#include "node.h"
#include "proposer.h"

class replica {
private:
    mutex m;
    // server info
    int port;
    std::string host;

    // replica id
    size_t id;

    // current view number that this replica thinks it is in
    size_t cur_view_num;

    // total number of replicas in the system
    size_t num_replicas = 0;

    network net;

    // list of replicas in the system
    std::vector<node> replicas;
    std::map<int, node> seq_to_client_map;

    void handle_msg(Message*);

    /* Adds all the replicas in the system to the receiver list of the reply message */
    void make_broadcast(Message *reply);

    /* Replica can take up any one of the following roles */
    Acceptor acceptor;
    Proposer proposer;
    Learner learner;

public:
    bool is_primary(int view_num);

    replica(int _port, std::string _host, int _id, std::string _config_file);
    void start();
};

#endif
