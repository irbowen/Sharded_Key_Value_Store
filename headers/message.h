
#ifndef message_h
#define message_h

#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

#define MAXBUFLEN 65536
#define LOCALHOST "127.0.0.1"

#define DEBUGZ
#ifdef DEBUGZ
    #define COUT std::cout
#else
    #define COUT while(0) std::cout
#endif

const string NO_OP = "NO_OP";
struct view_val {
    int view_num;
    string value;
    view_val(int n, string v) : view_num(n), value(v) {}
    view_val() {view_num = -1; value = NO_OP;}
};

struct node {
    int port;
    std::string host;
    // any other info regarding a server
    node(int _port, std::string _host) : port(_port), host(_host) {}
    node(){}
};

enum MessageType {
    NO_ACTION = 0,
    START_PREPARE = 1,
    PREPARE = 2,
    PREPARE_ACCEPT = 3,
    PREPARE_REJECT = 4,
    PROPOSE = 5,
    PROPOSE_ACCEPT = 6,
    PROPOSE_REJECT = 7,
    ACCEPT_VALUE = 8,
    PROPOSAL_LEARNT = 9,
    PUT = 10,
    GET = 11,
    DELETE = 12,
    ADD_SHARD = 13
};

class Message {
    char div_char = ':';
public:
    MessageType msg_type = MessageType::NO_ACTION;
    int view_num = -1;
    int seq_num = 0;
    std::string key = "";
    std::string value = "";
    node sender;
    std::vector<node> receivers;
    std::vector<view_val> acceptor_state;
    std::string serialize();
    void deserialize(std::string in);
    std::string get_key();
    std::string get_value();
    std::string get_client_id();
    int get_client_seq_num();
    node get_client_node();
};

#endif /* message_h */
