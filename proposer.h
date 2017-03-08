
#ifndef proposer_h
#define proposer_h

#include <cstdio>
#include <string>
#include <map>
#include "message.h"
#include "node.h"

class Proposer{
private:
    std::map<int, size_t> count;
    
    size_t quorum;
    int proposal_number;
    std::vector<node> replicas;
    
public:
    /* The value to be proposed */
    std::string to_propose;
    
    void init(vector<node> _replicas);
    Message* start_prepare(int proposal_number, int seq_num);
    
    Message* prepare_accept(int n_a, std::string value);
    
    Message* prepare_reject(int n_p, int seq_num);
    
    Message* propose_accept(int n);
    
    Message* propose_reject(int n_p, int seq_num);
};
#endif /* proposer_h */
