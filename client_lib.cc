
#include <iostream>
#include <string>
#include <vector>

#include "client_lib.h"

#define CLIENT_PORT	6023 	// Assume one client for now

client_lib::client_lib(int _port, string _host) : port(_port), host(_host), net(_port, _host) {
    cur_view_num = 0;
    client_seq_num = 0;
}

void client_lib::add_chat_message(std::string chat_message){
    Message msg;
    msg.msg_type = MessageType::START_PREPARE;
    msg.view_num = cur_view_num;

    string true_value = chat_message 
        + "#" + to_string(port)
        + "#" + host
        + "#" + to_string(client_seq_num);

    msg.value = true_value;

    msg.sender = node(port, host);

    int replica_start_port = 8000;
    /* TEST MODE - adding 1 client that send messages to the 3 replicas*/
    for(int i = 0; i < 3; i++){
        node n;
        n.port = replica_start_port + i;
        n.host = LOCALHOST;
        msg.receivers.push_back(n);
    }

    // the client will keep trying until success
    while (true) {
        msg.view_num = cur_view_num;
        net.sendto(&msg);
        Message *reply = net.recv_from_with_timeout();
        if (reply != nullptr && reply->msg_type == MessageType::PROPOSAL_LEARNT) {
            // the client library blocks until success so a return is a success to the client
            delete(reply);
            COUT << chat_message << " has been added to the chat log" << endl;
            client_seq_num++;
            return;
        }
        cur_view_num += 1;
    }

    return;
}
