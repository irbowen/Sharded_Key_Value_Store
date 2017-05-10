
#ifndef acceptor_h
#define acceptor_h

#include <string>

#include "message.h"
#include "environment.h"

class Acceptor {
private:
    int cur_view = -1;
    Environment* env_;
    std::string value = std::string();
    std::vector<view_val> accepted_values;
public:
    void init(Environment* env);
    Message* accept_prepare_msg(int in_view);
    Message* accept_propose_msg(int in_view, std::string value, int seq_num);
};

#endif
