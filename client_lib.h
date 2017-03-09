
#ifndef CLIENT_LIB_H
#define CLIENT_LIB_H

#include <iostream>
#include <string>
#include <vector>

#include "replica.h"
#include "message.h"
#include "network.h"
#include "node.h"

class client_lib {
private:
	int port;
	std::string host;
    std::string current_primary;
    std::string find_master();
    uint cur_view_num = 0;
    int client_seq_num;
    network net;
public:
    client_lib(int _port, string _host);
    // Blocking until success, may retry multiple times under the hood
    // Primary, upon receiving learn msg itself, will reply to client
    void add_chat_message(std::string msg);
};

#endif
