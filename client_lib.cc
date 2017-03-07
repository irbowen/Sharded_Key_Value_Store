
#include <iostream>
#include <string>
#include <vector>

#include "client_lib.h"

client_lib::client_lib() : net("127.0.0.1", 5000) {
  net.init();
    cur_view_num = 0;
}

void client_lib::add_chat_message(std::string chat_message){
    Message msg;
    msg.msg_type = MessageType::START_PREPARE;

    int timeout = 1;
    bool got_response = false;

    // first, try to send to the primary and see if it works
    msg.value = chat_message;
    node n;
    n.port = 2000;
    n.host = "127.0.0.1";
    msg.receivers.push_back(n);
    net.sendto(&msg);

    while (!got_response) {
        // send the message as a broadcast with timeout = timeout, if no response, try again
        // wait in recv loop
        timeout *= 2;
        // Increment the view number
    }
    // the client's chat_message has been added
    return;
}

