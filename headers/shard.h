
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
    int port_;
    std::string host_;
    std::string config_filename_;
    std::vector<node> replicas_;
    int client_seq_num_ = 0;
    std::queue<Message*> msg_queue_;
    std::mutex m;
    std::condition_variable cv;
    network net_;
    int cur_view_num_ = 0;
public:
    Shard(int port, std::string host, std::string config_filename);
    void run();
    /* These functions assume that this shard has the key that you want */
    void register_msg(Message* msg);
    std::string get(std::string key);
    void put(std::string key, std::string value);
};

#endif
