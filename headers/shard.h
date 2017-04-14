
#ifndef shard_h
#define shard_h

#include <string>
#include <fstream>
#include <vector>

#include "message.h"
#include "network.h"

class Shard {
private:
	std::string config_filename_;
    std::vector<node> replicas_;
    int client_seq_num_ = 0;
public:
    Shard(std::string config_filename);
    /* These functions assume that this shard has the key that you want */
    void handle_msg();
    std::string get(std::string key);
    void put(std::string key, std::string value);
};

#endif
