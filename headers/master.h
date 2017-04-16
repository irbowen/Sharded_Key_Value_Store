
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

    int tolerated_failures_;

    /* The master uses this to communicate
       with the clients */
    network net_;
    network move_net;
public:
    /* Init and shit */
    Master(int port, std::string host, std::string master_config_file);

    /* Generic network code */
    void recv();
    void handle_msg(Message* message);

    int get_shard_id(Message* message);
};

#endif
