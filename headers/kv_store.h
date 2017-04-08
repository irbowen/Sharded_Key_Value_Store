
#ifndef kv_store_h
#define kv_store_h

#include "learner.h"
#include "message.h"
#include "network.h"

class KV_Store {
private:
    Learner* learner_;
    int port_;
    std::string host_;
    std::vector<node> replicas_;

public:
    void init(Learner* learner, int port, std::string host, std::string config_filename);
    Message* handle_get_msg(Message* get_msg);
    Message* handle_put_msg(Message* put_msg);
};

#endif
