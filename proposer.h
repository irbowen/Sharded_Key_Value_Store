
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

    /* Did this primary just become the new primary === Fix required */
    bool is_new_primary = false;

    void init(vector<node> _replicas, int _id);

    // If we have already reached a quorum for this view_num
    // there is no need to run the firs step of paxos again
    bool reached_quroum(int view_num);

    // Create a PREPARE msg
    Message* handle_prepare(int view_num);

    // Create a PROPOSE msg
    Message* handle_prepare_accept(std::vector<view_val> acceptor_state, int view_num, std::string value);

    Message* handle_prepare_reject(int view_num);

    Message* handle_propose_accept(int view_num);

    Message* handle_propose_reject(int view_num);
};
#endif /* proposer_h */
