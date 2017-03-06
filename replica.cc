
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>

#include "replica.h"
#include "json.h"
#include "message.h"

using namespace std;

#define MAXBUFLEN 100

/* Setting up the replica with the provided port and host */
replica::replica(int port, std::string host) {
    
    this->port = port;
    this->host = host;
}

/* Start listening on the provided port and host */
void replica::start(){
    int serverfd = 1, opt_val = 1;
    serverfd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(serverfd >= 0);
    char buf[MAXBUFLEN];
    memset(&buf, '\0' ,MAXBUFLEN );
    struct sockaddr_in addr;
    memset(&addr, 0 , sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    
    assert(setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) == 0);
    assert(::bind(serverfd, (struct sockaddr*) &addr, sizeof(addr)) == 0);
    socklen_t addr_len = sizeof(addr);
    assert(getsockname(serverfd, (struct sockaddr *) &addr, &addr_len) == 0);
    
    struct sockaddr_storage their_addr;
    size_t numbytes;
    while (true) {
        printf("listener: waiting to recvfrom...\n");
        numbytes = recvfrom(serverfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len);
        if (numbytes == 0) {
            exit(1);
        }
        //string tmp(buf);
        Message *msg = (Message*) buf;
        std::thread t(&replica::handle_msg, this, *msg);
        t.detach();
    }
}

/* Handle the given message */
void replica::handle_msg(Message message) {
    cout << "Got: " << message.value << endl;

    Message reply;
    switch(message.msg_type){
        case MessageType::NO_ACTION:
            // do nothing in this case
            break;
        case MessageType::START_PREPARE:
            reply = proposer.start_prepare(message.prop_number);
            break;
        case MessageType::PREPARE:
            reply = acceptor.prepare(message.prop_number);
            break;
        case MessageType::PREPARE_ACCEPT:
            reply = proposer.prepare_accept(message.n_a, message.value);
            break;
        case MessageType::PREPARE_REJECT:
            reply = proposer.prepare_reject(message.n_p);
            break;
        case MessageType::PROPOSE:
            reply = acceptor.propose(message.prop_number, message.value);
            break;
        case MessageType::PROPOSE_ACCEPT:
            reply = proposer.propose_accept(message.prop_number);
            break;
        case MessageType::PROPOSE_REJECT:
            reply = proposer.propose_reject(message.n_p);
            break;
        case MessageType::BRDCST_LEARNERS:
            break;
    }
    
    // If client_req msg
    // skip
    
    // Incoming: prepare
    // Arguments: n
    // Acting as: Acceptor
    // Check if you've seen an n higher than this one
    // Outgoing: a prepare_accept
    //    OR prepare_reject
    
    // Incoming: prepare_accept
    // Arguments: n_a, v_a (could be null)
    // Acting as: Proposer
    // Increment the count of # of prepare accepts you have gotten
    // Outgoing: If this is more than a majority, send propose_value(n, v)
    
    // Incoming: prepare_reject
    // Arguments: n_p
    // Acting as: Proposer
    // If we got a higher n value, update our internal storage with this new value
    // Outgoing:
    
    // Incoming: propose
    // Arguments: n, v
    // Acting as: Acceptor
    // Outgoing: propose_accept(n) + broadcast_to_learners(n, v),
    //    OR propose_reject(n)
    
    // Incoming: propose_accept
    // Arguments: n
    // Acting as: Proposer
    // Outgoing:
    
    // Incoming: propose_reject
    // Arguments: n_p
    // Acting as: Proposer
    
    // Incoming: broadcast_to_learners
    // Arguments: n, v
    // Acting as: Learner
    // Outgoing: potenial client response
    
    
}


