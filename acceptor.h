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
    int cur_view = -1, id = 0;
    size_t num_replicas = 0;
    std::string value = std::string();
    // needs chat log
public:
    std::vector<ViewValue> accepted_values;
	  void init(size_t _num_replicas, int _id);
    Message* prepare(int in_view);
    Message* propose(int in_view, std::string value);
};
#endif /* acceptor_h */
