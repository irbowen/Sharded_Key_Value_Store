
#ifndef proposer_h
#define proposer_h

#include <cstdio>
#include <string>
#include <map>
#include "message.h"
#include "node.h"

<<<<<<< HEAD
class Proposer{
private:
=======
class Proposer {
  private:
>>>>>>> 64f30446c74d293fe1f9ac06e8204ee7ad3ff9c1
    std::map<int, int> count;
    
    size_t quorum;
    int proposal_number;
    std::vector<node> replicas;
    
public:
    /* The value to be proposed */
    std::string to_propose;
    
    void init(vector<node> _replicas);
    Message* start_prepare(int proposal_number);
    
    Message* prepare_accept(int n_a, std::string value);
    
    Message* prepare_reject(int n_p);
    
    Message* propose_accept(int n);
    
    Message* propose_reject(int n_p);
};
#endif /* proposer_h */
