
#ifndef message_h
#define message_h

#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "node.h"

using namespace std;

#define MAXBUFLEN 2048
#define LOCALHOST "127.0.0.1"

struct ViewValue {
  int view_num;
  std::string value;
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
    BRDCST_LEARNERS = 8,
    PROPOSAL_LEARNT = 9
};

class Message {
public:
    MessageType msg_type = MessageType::NO_ACTION;
    size_t view_num = -1;
    int seq_num = 0;
    std::string value = "";
    node sender;
    std::vector<node> receivers;
    std::vector<ViewValue> acceptor_state;
    std::string serialize();
    void deserialize(std::string in);
};

#endif /* message_h */
