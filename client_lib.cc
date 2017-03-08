
#include <iostream>
#include <string>
#include <vector>

#include "client_lib.h"

client_lib::client_lib() : net(5000, "127.0.0.1") {
  cur_view_num = 0;
}

void client_lib::add_chat_message(std::string chat_message){

  int timeout = 1;
  bool got_response = false;

  Message msg;
  msg.msg_type = MessageType::START_PREPARE;
  msg.n_a = 1;
  msg.n_p = 2;
  msg.prop_number = 10;
  msg.value = chat_message;
  // setup sender
  node s;
  s.port = 5000;
  s.host = "127.0.0.1";
  msg.sender = s;
  // setup receivers
  node n;
  n.port = 9000;
  n.host = "127.0.0.1";
  msg.receivers.push_back(n);
  node n2;
  n2.port = 9001;
  n2.host = "127.0.0.1";
  msg.receivers.push_back(n2);
  // Send the thing
  net.sendto(&msg);

  return;
  //TODO
  while (!got_response) {
    // send the message as a broadcast with timeout = timeout, if no response, try again
    // wait in recv loop
    timeout *= 2;
    // Increment the view number
  }
  // the client's chat_message has been added
  return;
}

