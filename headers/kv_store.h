
#ifndef kv_store_h
#define kv_store_h

#include "learner.h"
#include "message.h"
#include "network.h"

class KV_Store {
private:
    Learner* learner_;
    int replica_id_;
    int port_;
    std::string host_;
    std::vector<node> replicas_;

public:
    /* Setup and shit */
    KV_Store(int replica_id, int port, std::string host, std::vector<node> replicas);
    void init(Learner* learner);

    /* Listen for messages */
    void kv_recv();

    /* The key value stuff! */
    void handle_kv_msg(Message* message);
    Message* handle_get_msg(Message* get_msg);
    Message* handle_put_msg(Message* put_msg);
};

#endif
