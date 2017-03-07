#include "network.h"

network::network(std::string _host, int _port) : host(_host), port(_port) {
    // Set up the socket for this communication
    int opt_val = 1;
    serverfd = 1;
    serverfd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(serverfd >= 0);

    // Set up the addr info for this machine/host
    struct sockaddr_in addr;
    memset(&addr, 0 , sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host.c_str());
    addr.sin_port = htons(port);

    // Allow us to resuse address
    assert(setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) == 0);

    // Set the timeout to 100ms
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    assert(setsockopt(serverfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == 0);

    // Bind to the port
    assert(::bind(serverfd, (struct sockaddr*) &addr, sizeof(addr)) == 0);
    addr_len = sizeof(addr);
    assert(getsockname(serverfd, (struct sockaddr *) &addr, &addr_len) == 0);
    // At this point, the socket is ready to go
}

Message* network::recvfrom() {
    // Inti an empty buffer to hold incoming msg data
    //char buf[MAXBUFLEN];
    void *buf;
    memset(&buf, '\0' ,MAXBUFLEN );

    // Vars needed to recv
    struct sockaddr_storage their_addr;
    size_t numbytes;

    while (true) {
        cout << "listener: waiting to recvfrom...\n";
        numbytes = ::recvfrom(serverfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr*) &their_addr, &addr_len);
        // If there is an error, move on and log
        if (numbytes == 0) {
            cout << "There was an error getting data on the incoming socket\n";
            continue;
        }
        return (Message*)buf;
    }
}

void network::sendto(Message* message) {
    struct sockaddr_in to_addr;
    memset(&to_addr, 0 , sizeof(to_addr));
    for(auto serv : message->receivers){
        to_addr.sin_family = AF_INET;
        to_addr.sin_port = serv.port;
        inet_aton("127.0.0.1" , &to_addr.sin_addr);
        ::sendto(serverfd, &message, sizeof(message), 0, (struct sockaddr *)&to_addr, sizeof(to_addr));
    }
    //to_addr.sin_port = message->port;
    // Look into the message, get the destination from the message
    // Fill in addr using this info, and then fire it off
    // sendto(socket, &message, sizeof(message), 0, (struct sockaddr *)&addr, sizeof(sockaddr));
}
