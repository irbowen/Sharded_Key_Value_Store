//
//  acceptor.h
//  ChatService
//
//  Created by Pranav Ramarao on 3/5/17.
//  Copyright © 2017 Pranav Ramarao. All rights reserved.
//

#ifndef acceptor_h
#define acceptor_h

#include <stdio.h>
#include <string>

class Acceptor {
private:
    int n_a;
    int n_p;
    std::string accepted_value;
public:
    void prepare(int n);
    void propose(int n, std::string value);
};
#endif /* acceptor_h */
