//
//  acceptor.h
//  ChatService
//
//  Created by Pranav Ramarao on 3/5/17.
//  Copyright © 2017 Pranav Ramarao. All rights reserved.
//

#ifndef acceptor_h
#define acceptor_h

#include <string>
#include "message.h"

class Acceptor {
private:
    int n_a = -1;
    int n_p = -1;
    std::string value = std::string();
public:
    Message* prepare(int n);
    Message* propose(int n, std::string value);
};
#endif /* acceptor_h */
