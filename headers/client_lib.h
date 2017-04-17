
#ifndef CLIENT_LIB_H
#define CLIENT_LIB_H

#include <string>

#include "message.h"
#include "network.h"

class client_lib {
private:
    int master_port_, client_port_;
    std::string master_host_, client_host_;
    network net_;
public:
    client_lib(int master_port, std::string master_host, int client_port, std::string client_host);
    std::string get(std::string key);
    void put(std::string key, std::string value);
    void delete_key(std::string key);
    void add_shard(std::string config);

};

#endif
