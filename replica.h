
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
    // server info
    int port;
    std::string host;
    
    // replica id
    int id;
    
    // current view number that this replica thinks it is in
    int cur_view_num;
    
    // total number of replicas in the system
    size_t num_replicas = 0;
    
    network net;
    
    // list of replicas in the system
    std::vector<node> replicas;
    
    void handle_msg(Message*);
    
    /* Adds all the replicas in the system to the receiver list of the reply message */
    void add_all_to_receiver_list(Message *reply);
    
    /* Replica can take up any one of the following roles */
    Acceptor acceptor;
    Proposer proposer;
    Learner learner;
    
public:
    replica(int _port, std::string _host, int _id, std::string _config_file);
    void start();
};

#endif
