
#ifndef kv_store_h
#define kv_store_h

#include "environment.h"
#include "learner.h"
#include "message.h"

class KV_Store {
    /* A class that runs on top of the paxos instance, and can service basic
       key-value operations (put, get) consistently in the face of failures */
private:
    Learner* learner_;
    Environment* env_;
    bool is_primary(Message* msg);

public:
    KV_Store(Environment* env, Learner* learner);

    /* The dispatcher function */
    Message* handle_msg(Message* message);

    /* The key value stuff! */
    Message* handle_get_msg(Message* get_msg);
    Message* handle_put_msg(Message* put_msg);
    Message* handle_get_all_keys(Message* get_msg);
};

#endif
