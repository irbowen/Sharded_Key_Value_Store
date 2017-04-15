
#include "../headers/master.h"
#include "../headers/message.h"

/* Setting up the replica with the provided port and host */
Master::Master(int port, string host, string master_config_file)
    : port_(port), host_(host), net_(port, host) 
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
    return 0;
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
        // TODO handle moves - this only works for put/get
    }
}

