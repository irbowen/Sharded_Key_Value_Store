#include "network.h"

network::network(int _port, std::string _host) :
port(_port), host(_host) {
    // Set up the socket for this communication
    int opt_val = 1;
    serverfd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(serverfd > 0);
    
    // Set up the addr info for this machine/host
    struct sockaddr_in addr;
    memset(&addr, 0 , sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host.c_str());
    //addr.sin_addr.s_addr = htonl(INADDR_ANY);
    cout << "Port: " << port << endl;
    addr.sin_port = htons(port);
    
    // Allow us to resuse address
    assert(setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) == 0);
    assert(::bind(serverfd, (struct sockaddr*) &addr, sizeof(addr)) == 0);
    addr_len = sizeof(addr);
    cout << "@@@ socketfd: " << serverfd << endl;
    cout << "@@@ host " << ntohl(addr.sin_addr.s_addr) << endl;
    cout << "@@@ port " << ntohs(addr.sin_port) << endl;
}

/* Block recv on socket */
Message* network::recv_from() {
    char* buf = new char[MAXBUFLEN];
    memset(buf, '\0', MAXBUFLEN);
    struct sockaddr_storage their_addr;
    ssize_t numbytes = 0;
    
    while (true) {
        cout << "listener: waiting to recv_from...\n";
        numbytes = recvfrom(serverfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr*) &their_addr, &addr_len);
        // cout << "Num bytes: " << numbytes << endl;
        if (numbytes < 0) {
            cout << "There was an error getting data on the incoming socket\n";
            continue;
        }
        if (numbytes == 0) {
            cout << "There was no data\n";
            continue;
        }
        string tmp(buf);
        Message* msg = new Message();
        msg->deserialize(tmp);
        return msg;
    }
}

void network::sendto(Message* message) {
    
    struct sockaddr_in to_addr;
    memset(&to_addr, 0 , sizeof(to_addr));
    for (auto serv : message->receivers) {
        cout << "suh dude\n";
        to_addr.sin_family = AF_INET;
        to_addr.sin_port = htons(serv.port);
        inet_aton("127.0.0.1" , &to_addr.sin_addr);
        cout << "@@@ host " << ntohl(to_addr.sin_addr.s_addr) << endl;
        cout << "@@@ port " << ntohs(to_addr.sin_port) << endl;
        auto msg_buf = message->serialize();
        auto retvalue = ::sendto(serverfd, msg_buf.c_str(), msg_buf.size(), 0,
                                 (struct sockaddr *)&to_addr, sizeof(to_addr));
        cout << "ret value " << retvalue << endl;
        cout << "errno value " << errno << endl;
        perror("sendto");
    }
    //to_addr.sin_port = message->port;
    // Look into the message, get the destination from the message
    // Fill in addr using this info, and then fire it off
    // sendto(socket, &message, sizeof(message), 0, (struct sockaddr *)&addr, sizeof(sockaddr));
}

/*
 Message* network::recv_from_with_timeout() {
 struct timeval tv;
 tv.tv_sec = 0;
 tv.tv_usec = 100000;
 assert(setsockopt(serverfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == 0);
 void *buf;
 memset(&buf, '\0' ,MAXBUFLEN);
 
 // Vars needed to recv
 struct sockaddr_storage their_addr;
 size_t numbytes;
 
 while (true) {
 cout << "listener: waiting to recv_from_with_timeout...\n";
 numbytes = recvfrom(serverfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr*) &their_addr, &addr_len);
 if (numbytes <= 0) {
 cout << "There was an error getting data on the incoming socket\n";
 continue;
 }
 cout << "Got a msg: " << buf;
 return (Message*)buf;
 }
 }
 */
