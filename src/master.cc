
#include "../headers/master.h"
#include "../headers/message.h"

/* Setting up the replica with the provided port and host */
Master::Master(int port, string host, string master_config_file)
    : port_(port), host_(host), net_(port, host) 
{
    string h, p, rep_id, f;
    ifstream config_fs(master_config_file);
    config_fs >> h >> p;
    assert(stoi(p) == port);
    assert(h == host);
    config_fs >> f;
    tolerated_failures_ = stoi(f);
    // TODO: read in shards
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
    // TODO handle moves - this only works for put/get
    /* Figure out which shard is responsible for this key, and send the call to it */
    int shard_id = get_shard_id(message);
    shards_.at(shard_id).register_msg(message);
}

