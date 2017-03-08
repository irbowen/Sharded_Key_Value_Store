//
//  node
//  ChatService
//
//  Created by Pranav Ramarao on 3/6/17.
//  Copyright © 2017 Pranav Ramarao. All rights reserved.
//

#ifndef node_h
#define node_h

struct node{
    int port;
    std::string host;
    // any other info regarding a server
    node(int _port, std::string _host) : port(_port), host(_host) {}
    node(){}
};

#endif /* node_h */
