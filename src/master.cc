
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


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
    while (config_fs >> h >> p >> rep_id) {
        replicas.push_back(node(stoi(p), h));
    }
}

void Master::recv() {
    while (true) {
        Message* msg = net_.recv_from();
        handle_msg(msg);
    }
}

void Master::get_shard_id(Message* message) {
    // TODO. For now, all on shard 0
    return 0;
}

void Master::handle_msg(Message* message) {
    /* Figure out which shard is responsible for this key, and send the call to it */
    int shard_id = get_shard_id(message);
    Message* reply = shards_.at(shard_id).handle_msg(message);
    if (reply != nullptr && reply->msg_type != MessageType::NO_ACTION) {
        reply->sender.host = host_;
        reply->sender.port = port_;
        cout << "[Master] Sending reply: " << reply->serialize() << endl;
        net_.sendto(reply);
    }
    delete(message);
    delete(reply);
    // See if there are any queued up messages for this shard
    Message* reply = shards_.at(shard_id).check_queue();
    if (reply != nullptr && reply->msg_type != MessageType::NO_ACTION) {
        reply->sender.host = host_;
        reply->sender.port = port_;
        cout << "[Master] Sending reply: " << reply->serialize() << endl;
        net_.sendto(reply);
    }
    delete(reply);
}
