
#ifndef PAXOS_H_
#define PAXOS_H_

#include <iostream>
#include <string>
#include <condition_variable>
#include <thread>
#include <vector>

#include "acceptor.h"
#include "proposer.h"
#include "learner.h"

class replica {
private:
    int port;
    std::string host;
    std::string config_file;
    void handle_msg(std::string);
    
    Acceptor acceptor;
    Proposer proposer;
    Learner learner;
    
public:
    replica(int port, std::string host);
    void start();
};

#endif
