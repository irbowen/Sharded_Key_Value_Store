#include "../headers/network.h"

network::network(int _port, std::string _host) : port(_port), host(_host) {
    // Set up the socket for this communication
    int opt_val = 1;
    serverfd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(serverfd > 0);

    // Set up the addr info for this machine/host
    struct sockaddr_in addr;
    memset(&addr, 0 , sizeof(addr));
    addr.sin_family = AF_INET;
    //addr.sin_addr.s_addr = inet_addr(host.c_str());
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    COUT << "Port: " << port << endl;
    addr.sin_port = htons(port);

    // Allow us to resuse address
    assert(setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) == 0);
    assert(::bind(serverfd, (struct sockaddr*) &addr, sizeof(addr)) == 0);
    addr_len = sizeof(addr);
    COUT << "@@@ socketfd: " << serverfd << endl;
    COUT << "@@@ host " << ntohl(addr.sin_addr.s_addr) << endl;
    COUT << "@@@ port " << ntohs(addr.sin_port) << endl;
}

/* Block recv on socket */
Message* network::recv_from() {
    char* buf = new char[MAXBUFLEN];
    memset(buf, '\0', MAXBUFLEN);
    struct sockaddr_storage their_addr;
    ssize_t numbytes = 0;

    while (true) {
        //COUT << "listener: waiting to recv_from...\n";
        numbytes = recvfrom(serverfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr*) &their_addr, &addr_len);
        // COUT << "Num bytes: " << numbytes << endl;
        if (numbytes < 0) {
            COUT << "There was an error getting data on the incoming socket\n";
            continue;
        }
        if (numbytes == 0) {
            COUT << "There was no data\n";
            continue;
        }
        string tmp(buf);
        Message* msg = new Message();
        msg->deserialize(tmp);
        return msg;
    }
}

Message* network::recv_from_with_timeout() {

    char* buf = new char[MAXBUFLEN];
    memset(buf, '\0', MAXBUFLEN);
    struct sockaddr_storage their_addr;
    ssize_t numbytes = 0;

    while (true) {
        struct timeval tv;
        tv.tv_sec = micro_second_delay / MICRO_PER_SECOND;
        tv.tv_usec = micro_second_delay % MICRO_PER_SECOND;
        //tv.tv_sec = 10000000;
        assert(setsockopt(serverfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == 0);
        COUT << "listener: waiting to recv_from with delay of " << micro_second_delay << "\n";
        numbytes = recvfrom(serverfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr*) &their_addr, &addr_len);
        // COUT << "Num bytes: " << numbytes << endl;
        if (numbytes <= 0) {
            COUT << "There was no data, time to double timeout\n";
            micro_second_delay *= 2;
            delete[] buf;
            return nullptr;
        }
        // if control reachers here, we can go ahead and reset the delay
        micro_second_delay = START_TIMEOUT;
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
        to_addr.sin_family = AF_INET;
        to_addr.sin_port = htons(serv.port);
        inet_aton(serv.host.c_str(), &to_addr.sin_addr);
        //COUT << "@@@ host " << ntohl(to_addr.sin_addr.s_addr) << endl;
        //COUT << "@@@ port " << ntohs(to_addr.sin_port) << endl;
        auto msg_buf = message->serialize();
        //auto retvalue =
        ::sendto(serverfd, msg_buf.c_str(), msg_buf.size(), 0,
                (struct sockaddr *)&to_addr, sizeof(to_addr));
        //COUT << "ret value " << retvalue << endl;
        //COUT << "errno value " << errno << endl;
    }
}
