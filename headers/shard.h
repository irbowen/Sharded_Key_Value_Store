
#ifndef shard_h
#define shard_h

#include <condition_variable>
#include <fstream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "message.h"
#include "network.h"

class Shard {
private:
    /* The port and host that this shard manager will use
       to communicate with the shard */
    int port_;
    std::string host_;
    /* The config file which describes the key/value/paxos replicas
       in the shard */
    std::string config_filename_;
    std::vector<node> replicas_;
    /* Only incremented on failure, so we can promote a new
       leader in the shard */
    int cur_view_num_ = 0;
    /* Should be incremented on every request to this shard */
    int client_seq_num_ = 0;
    /* Used to handle and sync communication with the shard */
    network net_;
    std::queue<Message*> msg_queue_;
    std::mutex m;
    std::condition_variable cv;
public:
    Shard(int port, std::string host, std::string config_filename);
    void run();
    /* These functions assume that this shard has the key that you want */
    void register_msg(Message* message);
    Message* handle_get(std::string key, node sender);
    Message* handle_put(std::string key, std::string value, node sender);
    Message* handle_delete(std::string key, node sender);
};

#endif
