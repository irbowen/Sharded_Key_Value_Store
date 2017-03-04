
#ifndef PAXOS_H_
#define PAXOS_H_

#include <iostream>
#include <string>
#include <condition_variable>
#include <thread>
#include <future>
#include <algorithm>
#include <vector>

class paxos {
private:
  std::string config_file;
  void process_request(int32_t);
public:
  void setup_server_for_requests(int, std::string);
};

#endif
