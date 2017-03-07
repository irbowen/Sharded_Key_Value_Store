
#include <arpa/inet.h>
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

#include "json.h"
#include "message.h"

using namespace std;

#define MAXBUFLEN 100

class network {

  private:
    std::string host;
    int serverfd = 1, port = 1;
    socklen_t addr_len;

  public:
    void init();
    network(std::string ip_addr, int socket);
    void sendto(Message* message);
    Message* recv_from();
    Message* recv_from_with_timeout();
};
