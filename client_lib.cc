
#include <iostream>
#include <string>
#include <vector>

#include "client_lib.h"

#define LOCALHOST "127.0.0.1"
#define CLIENT_PORT	6023 	// Assume one client for now
client_lib::client_lib() : net(LOCALHOST, CLIENT_PORT) {
    cur_view_num = 0;
}

void client_lib::add_chat_message(std::string chat_message){
    Message msg;
    msg.msg_type = MessageType::START_PREPARE;
    
    msg.value = chat_message;
    msg.prop_number = 0;
    /* TEST MODE - adding 1 client that send messages to the 3 replicas*/
    for(int i = 0; i < 3; i++){
        node n;
        n.port = 2000 + i;
        n.host = LOCALHOST;
        msg.receivers.push_back(n);
    }
    net.sendto(&msg);
    cout << chat_message << " has been added to the chat log" << endl;
    

//    int timeout = 1;
//    bool got_response = false;
//    while (!got_response) {
//        // send the message as a broadcast with timeout = timeout, if no response, try again
//        // wait in recv loop
//        timeout *= 2;
//        // Increment the view number
//    }
//    // the client's chat_message has been added
    return;
}

