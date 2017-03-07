//
//  message.h
//  ChatService
//
//  Created by Pranav Ramarao on 3/5/17.
//  Copyright © 2017 Pranav Ramarao. All rights reserved.
//

#ifndef message_h
#define message_h

#include <string>
#include <vector>
#include "node.h"

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
class Message{
    
public:
    std::string value = std::string();
    MessageType msg_type = MessageType::NO_ACTION;
    int n_a = -1;
    int n_p = -1;
    int prop_number = -1;

    node sender;
    std::vector<node> receivers;
};
#endif /* message_h */
