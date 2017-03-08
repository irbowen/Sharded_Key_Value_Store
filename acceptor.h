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
    int cur_view = 0, num_replicas = 0, id = 0;
    std::string value = std::string();
    // needs chat log
public:
	void init(int _view, int _num_replicas, int _id);
    Message* prepare(int n);
    Message* propose(int n, std::string value);
};
#endif /* acceptor_h */
