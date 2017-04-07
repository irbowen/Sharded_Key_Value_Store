
#include "../headers/kv_store.h"

KV_Store::KV_Store(Learner* l) {
    this->learner = l;
}

Message* KV_Store::handle_get_msg(Message* get_msg) {
    // Scan log and get data
}

Message* KV_Store::handle_put_msg(Message* put_msg) {
    // We can pretty much just respond right away
}
