
#ifndef replica_h
#define replica_h

#include <condition_variable>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "paxos.h"
#include "kv_store.h"
#include "environment.h"

class Replica {
private:
    Environment* env_;

    /* Each replicas runs an instance of paxos,
       which has an acceptor, learner, and proposer object */
    std::unique_ptr<Paxos> paxos_;

    /* Replica also runs the key value server in another thread */
    std::unique_ptr<KV_Store> kv_store_;

    /* Messages are dispatched to the correct class to handle */
    void handle_msg(Message*);

public:
    Replica(Environment* env, std::string holes_file);
    void start();
};

#endif
