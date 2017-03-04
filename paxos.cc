
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
void paxos::process_request(int32_t socket) {
  // Do something here
}

void paxos::setup_server_for_requests(int port, std::string host) {

  int serverfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (serverfd < 0)
    assert(false);
  char buf[MAXBUFLEN];
  memset(&buf, '\0' ,MAXBUFLEN );
  int opt_val = 1;
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

  cout << "\n@@@ port " << ntohs(addr.sin_port) << endl;

    struct sockaddr_storage their_addr;
    int numbytes;
  while (true) {
    printf("listener: waiting to recvfrom...\n");
    numbytes = recvfrom(serverfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len);
    if (numbytes < 0) {
      perror("recvfrom");
      exit(1);
    }
    printf("listener: packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    printf("listener: packet contains [%s] \n", buf);
  }
}

