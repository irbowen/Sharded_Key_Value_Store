
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

  if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) < 0)
    assert(false);
  if (bind(serverfd, (struct sockaddr*) &addr, sizeof(addr)) < 0)
    assert(false);
  socklen_t addr_len = sizeof(addr);
  if (getsockname(serverfd, (struct sockaddr *) &addr, &addr_len) < 0)
    assert(false);

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

