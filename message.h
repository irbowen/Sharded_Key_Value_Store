
#ifndef message_h
#define message_h

#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "node.h"

#define MAXBUFLEN 1024

using namespace std;

enum MessageType {
  NO_ACTION = 0,
  START_PREPARE = 1,
  PREPARE = 2,
  PREPARE_ACCEPT = 3,
  PREPARE_REJECT = 4,
  PROPOSE = 5,
  PROPOSE_ACCEPT = 6,
  PROPOSE_REJECT = 7,
  BRDCST_LEARNERS = 8
};

class Message {
  public:
    MessageType msg_type = MessageType::NO_ACTION;
    int n_a = -1;
    int n_p = -1;
    int prop_number = -1;
    std::string value = "";
    node sender;
    std::vector<node> receivers;
    std::string serialize();
};

#endif /* message_h */
