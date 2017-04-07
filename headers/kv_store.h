
#ifndef kv_store_h
#define kv_store_h

#include "../headers/learner.h"

class KV_Store {
private:
    Learner* learner;

public:
    KV_Store(Learner* _learner);
    Message* handle_get_msg(Message* get_msg);
    Message* handle_put_msg(Message* put_msg);
};

#endif
