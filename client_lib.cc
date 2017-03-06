#include <iostream>
#include <string>
#include <vector>
#include "client_lib.h"
#include "replica.h"
#include "message.h"

void client_lib::add_chat_message(std::string chat_message){
    Message msg;
    msg.msg_type = MessageType::START_PREPARE;

    int timeout = 1;
    bool got_response = false;

    // first, try to send to the primary and see if it works

    while(!got_response){
        // send the message as a broadcast with timeout = timeout, if no response, try again
        timeout *= 2;
        // Increment the view number
    }
    // the client's chat_message has been added
    return;
}

