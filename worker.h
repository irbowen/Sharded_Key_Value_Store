#ifndef WORKER_H_
#define WORKER_H_

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

class worker {

private:
  std::mutex startup_mutex_;
  std::condition_variable startup_cv_;
  bool startup_ready_ = false;
  std::string master_hostname_, worker_hostname_;
  int master_port_, worker_port_;
public:
worker(std::string master_hostname, int master_port,
    std::string worker_hostname, int worker_port);
void setup_worker();
void register_with_master();
void process_request(int32_t socket);

};

#endif
