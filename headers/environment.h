
#ifndef environment_h
#define environment_h

#include <fstream>
#include <string>
#include <vector>
#include "message.h"
#include "network.h"


/* Contains information about replicas, that can be easily passed
  down and injected into other classes */
class environment {
public:
    // Construct the environment object
    environment(int port, std::string host, int id, std::string config_file, network* net);
    // Node representing this server
    node server_;
    // This node's id in the replica list
    int replica_id_;
    // Total number of replicas in the system
    size_t num_replicas_ = 0;
    // List of all the replicas in the system
    std::vector<node> replicas_;
    // A pointer to the global network object used by this replica
    network* net_;
    // Turn a message into a broadcast message
    void convert_msg_to_broadcast(Message* msg);
};

#endif
