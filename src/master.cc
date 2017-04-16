
#include "../headers/master.h"
#include "../headers/message.h"
#include <ctype.h>
/* Setting up the replica with the provided port and host */
Master::Master(int port, string host, string master_config_file)
    : port_(port), host_(host), net_(port, host) , move_net(port - 1, host)
{
    /* The master reads in the host and port that it will listen on
       for client requests */
    string h, p, id, f;
    ifstream config_fs(master_config_file);
    /* Make sure the command line and config file agree */
    config_fs >> h >> p;
    assert(stoi(p) == port);
    assert(h == host);
    config_fs >> f;
    tolerated_failures_ = stoi(f);
    /* Read in each config file name. Each file describes on shard */
    int counter = 1;
    string file_name;
    while (config_fs >> file_name) {
        Shard* s = new Shard(port_ + counter, host_, file_name);
        thread t(&Shard::run, s);
        t.detach();
        shards_.push_back(s);
        counter++;
    }
}

void Master::recv() {
    while (true) {
        Message* msg = net_.recv_from();
        handle_msg(msg);
    }
}


int Master::get_shard_id(Message* message) {
    // TODO. For now, all on shard 0
    int shard_id = (int)(tolower(message->key[0])-'a') % shards_.size();
    cout << "assigned shard is " << shard_id << endl;
    return shard_id;


    // int ring_size = 300;
    // vector<int> ring(ring_size, 0);
}

void Master::handle_msg(Message* message) {
    /* Figure out which shard is responsible for this key,
       and send the call to it */
    if (message->msg_type == MessageType::GET ||
            message->msg_type == MessageType::PUT ||
            message->msg_type == MessageType::DELETE) {
        int shard_id = get_shard_id(message);
        shards_.at(shard_id)->register_msg(message);
    }
    if (message->msg_type == MessageType::ADD_SHARD) {
        int successor_id = 0; // get successor id
        int new_shard_id = 1; // get new shard id
        Message *get_all_message = new Message;
        get_all_message->sender = node(port_ - 1, host_);
        get_all_message->msg_type = MessageType::GET_KEYS;
        shards_.at(successor_id)->register_msg(get_all_message);
        Message *reply = move_net.recv_from();
        vector<string> keys = reply->all_keys;

        vector<string> to_move_keys;
        bool should_move = true;
        for(auto key: keys){
            // hash each get and decide which needs to go where
            if (should_move)
                to_move_keys.push_back(key);
        }

        for(auto key: to_move_keys){
            // get key_value from old shard
            Message *mess = new Message;
            mess->sender = node(port_ - 1, host_);
            mess->msg_type = MessageType::GET;
            mess->key = key;
            shards_.at(successor_id)->register_msg(mess);

            // put key_value to new shard
            mess = move_net.recv_from();
            mess->sender = node(port_ - 1, host_);
            mess->msg_type = MessageType::PUT;
            shards_.at(new_shard_id)->register_msg(mess);

            // delete key_value from old shard
            mess = move_net.recv_from();
            mess->key = key;
            mess->sender = node(port_ - 1, host_);
            mess->msg_type = MessageType::DELETE;
            shards_.at(successor_id)->register_msg(mess);
            mess = move_net.recv_from();
            delete(mess);
        }
        // note : the master is blocking on the above add shard operation
        // hence new requests from clients will be queued up in net_
        // but we use move_net for moving keys from shards
        
        // send success to client
        reply = new Message;
        reply->msg_type = MessageType::MASTER_ACK;
        reply->receivers.push_back(message->sender);
        net_.sendto(reply);
    }
}

