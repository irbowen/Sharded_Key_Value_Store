
#include <iostream>
#include <string>
#include <vector>

#include "../headers/client_lib.h"

using namespace std;

client_lib::client_lib(int master_port, string master_host, int client_port, string client_host) :
    master_port_(master_port), master_host_(master_host), client_port_(client_port), client_host_(client_host), net_(client_port, client_host)
{
}


string client_lib::get(string key) {
    Message msg;
    msg.msg_type = MessageType::GET;
    msg.key = key;
    msg.column= "column";
    msg.sender = node(client_port_, client_host_);
    msg.receivers.push_back(node(master_port_, master_host_));
    while (true) {
        net_.sendto(&msg);
        Message *reply = net_.recv_from_with_fixed_timeout();
        if (reply != nullptr && reply->msg_type == MessageType::MASTER_ACK) {
            string val = reply->value;
            delete(reply);
            return val;
        }
    }
}

void client_lib::put(string key, string value) {
    Message msg;
    msg.msg_type = MessageType::PUT;
    msg.key = key;
    msg.column= "column";
    msg.value = value;
    msg.sender = node(client_port_, client_host_);
    msg.receivers.push_back(node(master_port_, master_host_));
    while (true) {
        net_.sendto(&msg);
        Message *reply = net_.recv_from_with_fixed_timeout();
        if (reply != nullptr && reply->msg_type == MessageType::MASTER_ACK) {
            delete(reply);
            return;
        }
    }
}

void client_lib::delete_key(std::string key) {
    Message msg;
    msg.msg_type = MessageType::DELETE;
    msg.key = key;
    msg.column= "column";
    msg.sender = node(client_port_, client_host_);
    msg.receivers.push_back(node(master_port_, master_host_));
    while (true) {
        net_.sendto(&msg);
        Message *reply = net_.recv_from_with_fixed_timeout();
        if (reply != nullptr && reply->msg_type == MessageType::MASTER_ACK) {
            return;
        }
    }
}

void client_lib::add_shard(std::string config) {
    Message msg;
    msg.value = config;
    msg.msg_type = MessageType::ADD_SHARD;
    msg.sender = node(client_port_, client_host_);
    msg.receivers.push_back(node(master_port_, master_host_));
    while (true) {
        net_.sendto(&msg);
        Message *reply = net_.recv_from_with_fixed_timeout();
        if (reply != nullptr && reply->msg_type == MessageType::MASTER_ACK) {
            return;
        }
    }
}
