
#ifndef PAXOS_H_
#define PAXOS_H_

#include <iostream>
#include <string>
#include <condition_variable>
#include <thread>
#include <vector>

class paxos {
private:
  std::string config_file;
  void handle_msg(std::string);
public:
  void setup_server(int port, std::string host);
};

#endif
