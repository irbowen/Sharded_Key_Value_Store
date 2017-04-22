
#ifndef network_h_
#define network_h_

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

#include "message.h"

using namespace std;
#define MICROSECONDS 1000
#define START_TIMEOUT (100 * MICROSECONDS)
#define MICRO_PER_SECOND 1000000

class network {
private:
    int port;
    std::string host;
    int serverfd;
    socklen_t addr_len;
    int micro_second_delay = START_TIMEOUT;
    
public:
    network(int port, std::string ip_addr);
    void set_start_timeout_factor(int factor);
    void sendto(Message* message);
    Message* recv_from();
    Message* recv_from_with_timeout();
    Message* recv_from_with_fixed_timeout();
};

#endif
