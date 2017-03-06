//
//  message.h
//  ChatService
//
//  Created by Pranav Ramarao on 3/5/17.
//  Copyright © 2017 Pranav Ramarao. All rights reserved.
//

#ifndef message_h
#define message_h

enum MessageType {
    PREPARE = 0,
    PREPARE_ACCEPT = 1,
    PREPARE_REJECT = 2,
    PROPOSE = 3,
    PROPOSE_ACCEPT = 4,
    PROPOSE_REJECT = 5,
    BRDCST_LEARNERS = 6
};
class Message{
    
public:
    std::string value;
    MessageType msg_type;
    int n_a;
    int n_p;
    int n;
};
#endif /* message_h */
