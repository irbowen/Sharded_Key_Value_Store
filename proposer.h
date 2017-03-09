
#ifndef proposer_h
#define proposer_h

#include <cstdio>
#include <string>
#include <map>
#include "message.h"
#include "node.h"

class Proposer {
private:
    std::map<int, size_t> count;

    size_t quorum;
    int id;

    std::vector<node> replicas;

public:
    /* The value to be proposed */
    std::string to_propose;

    void init(vector<node> _replicas, int _id);

    // If we have already reached a quorum for this view_num
    // there is no need to run the firs step of paxos again
    bool reached_quroum(int view_num);

    // Create a PREPARE msg
    Message* start_prepare(int view_num);

    // Create a PROPOSE msg
    Message* prepare_accept(int view_num, std::string value);

    Message* prepare_reject(int view_num);

    Message* propose_accept(int view_num);

    Message* propose_reject(int view_num);
};
#endif /* proposer_h */
