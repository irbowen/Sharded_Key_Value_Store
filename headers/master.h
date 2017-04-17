
#ifndef master_h
#define master_h

#include <string>
#include <vector>

#include "message.h"
#include "network.h"
#include "shard.h"

class Master {
private:
    int port_;
    std::string host_;
    std::vector<Shard*> shards_;
    int ring_size;
    int tolerated_failures_;

    /* The master uses this to communicate
       with the clients */
    network net_;
    network move_net;
    std::vector<int> _ring;

    int get_successor(int hash);

public:
    /* Init and shit */
    Master(int port, std::string host, std::string master_config_file);

    int hash(string input);
    /* Generic network code */
    void recv();
    void handle_msg(Message* message);

    void handle_add_shard(Message *message);
    void create_shard(std::string config);

    int get_shard_id(Message* message);
};

#endif
