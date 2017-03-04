
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

#include "paxos.h"
#include "json.h"

using namespace std;

#define MAXBUFLEN 100

void paxos::handle_msg(std::string msg) {
  // Step 1. Parse msg into json
  // Step 2. Determine what kind of msg it is
  // Step 3. Call the correct function to hanlde it
  cout << "Got: " << msg << endl;
  
  // If client_req msg
  // skip


  // Incoming: prepare
  // Arguments: n
  // Acting as: Acceptor
  // Check if you've seen an n higher than this one
  // Outgoing: a prepare_accept or prepare_deny

  // Incoming: prepare_accept
  // Arguments: n_a, v_a (could be null)
  // Acting as: Proposer
  // Increment the count of # of prepare accepts you have gotten
  // Outgoing: If this is more than a majority, send propose_value(n, v)

  // Incoming: prepare_deny
  // Arguments: n_p
  // Acting as: Proposer
  // If we got a higher n value, update our internal storage with this new value
  // Outgoing:

  // Incoming: Propose
  // Arguments: n, v
  // Acting as: Acceptor
  // Outgoing: propose_accept(n), or propose_reject(n)

  // Incoming: Propose_accept
  // Arguments: n
  // Acting as: Proposer
  // Outgoing: decided(v)

  // Incoming: propose_accept
  // Arguments: n
  // Acting as: Proposer
  // Outgoing: decided(v)

  // Incoming: decided
  // Arguments: v
  // Acting as: Learner
  // Outgoing:
}

void paxos::setup_server(int port, std::string host) {

  int serverfd = 1, opt_val = 1;
  serverfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (serverfd < 0)
    assert(false);
  char buf[MAXBUFLEN];
  memset(&buf, '\0' ,MAXBUFLEN );
  struct sockaddr_in addr;
  memset(&addr, 0 , sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);

  assert(setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) == 0);
  assert(bind(serverfd, (struct sockaddr*) &addr, sizeof(addr)) == 0);
  socklen_t addr_len = sizeof(addr);
  assert(getsockname(serverfd, (struct sockaddr *) &addr, &addr_len) == 0);

  struct sockaddr_storage their_addr;
  int numbytes;
  while (true) {
    printf("listener: waiting to recvfrom...\n");
    numbytes = recvfrom(serverfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len);
    if (numbytes < 0) {
      exit(1);
    }
    string tmp(buf);
    std::thread t(&paxos::handle_msg, this, tmp);
    t.detach();
  }
}

