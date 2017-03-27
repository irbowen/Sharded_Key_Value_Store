
#include <iostream>
#include <string>
#include <vector>

#include "../headers/client_lib.h"

client_lib::client_lib(int _port, string _host, string config_filename) : port(_port), host(_host), net(_port, _host) {
    cur_view_num = 0;
    client_seq_num = 0;

    string h, p, rep_id;
    ifstream config_fs(config_filename);
    while (config_fs >> h >> p >> rep_id) {
        replicas.push_back(node(stoi(p), h));
    }
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

    for(auto& r : replicas){
        msg.receivers.push_back(r);
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
