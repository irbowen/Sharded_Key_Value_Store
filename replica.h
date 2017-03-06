
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

    /* Number of live replicas in the system: 2f + 1 */
    int tot_replicas;

    /* Get the majority number f + 1 */
    int get_quorum() { return (tot_replicas - 1) << 2; }

    void handle_msg(Message);

    /* Replica can take up any one of the following roles */
    Acceptor acceptor;
    Proposer proposer;
    Learner learner;
    
public:
    replica(int port, std::string host);
    void start();
};

#endif
