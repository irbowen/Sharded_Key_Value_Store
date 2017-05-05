
#ifndef paxos_h
#define paxos_h

#include <condition_variable>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "acceptor.h"
#include "learner.h"
#include "network.h"
#include "proposer.h"

class paxos {
private:
    // server info
    int port;
    std::string host;

    // replica id
    int id;

    // holes that we need to simulate
    vector<int> seq_holes;
    bool is_seq_hole(int seq);

    // holes that we need to simulate
    set<int> seen_views;

    // current view number that this replica thinks it is in
    int cur_view_num = -1;

    // total number of replicas in the system
    size_t num_replicas = 0;

    // list of replicas in the system
    std::vector<node> replicas;
    std::map<int, node> seq_to_client_map;

    /* Mapping from client_id to the latest client sequence number that got finished */
    std::map<string, int> client_progress_map;

    /* Paxos uses the following classes to maintain consistency */
    Acceptor acceptor;
    Proposer proposer;
    Learner learner;

public:
    Message* handle_msg(Message*);
    bool is_primary(int view_num);
    bool is_previous_view(int view_num);
    void make_broadcast(Message* msg);
    paxos(int _port, std::string _host, int _id,
      std::string _config_file, std::string _holes_file);
};

#endif
