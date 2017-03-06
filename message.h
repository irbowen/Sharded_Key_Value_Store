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
    PREPARE_ACCEPT 1
};
class Message{
    
    std::string message;
    MessageType msg_type;
};
#endif /* message_h */
