
#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <string>
#include <condition_variable>
#include <thread>
#include <future>
#include <algorithm>
#include <vector>

class server {
private:

  std::string config_file;
  std::string slave_config;
  void process_request(int32_t);
  void communicate_with_client(int32_t);
public:
  std::mutex startup_mutex;
  std::condition_variable startup_cv;
  bool startup_ready = false;

  void maintain_connection_with_client(int32_t);
  static void print_help_and_exit();

  void setup_server_for_requests(int, std::string);
};

#endif
