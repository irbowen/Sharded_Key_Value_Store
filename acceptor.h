
#ifndef acceptor_h
#define acceptor_h

#include <string>
#include "message.h"

class Acceptor {
private:
    int cur_view = -1, id = 0;
    size_t num_replicas = 0;
    std::string value = std::string();
    std::vector<view_val> accepted_values;
public:
    void init(size_t _num_replicas, int _id);
    Message* prepare(int in_view);
    Message* propose(int in_view, std::string value, int seq_num);
};
#endif /* acceptor_h */
