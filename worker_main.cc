
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include "flags.h"
#include "worker.h"

using namespace std;

void print_help_and_exit() {
  std::cout << "\nThis program takes a few command line arguments:\n";
  // TODO fill in this error msg
  exit(0);
}

int main(int argc, char *argv[]) {

  const flags::args args(argc, argv);

  auto help = args.get<bool>("help", false);
  if (help) {
    print_help_and_exit();
  }

  auto worker_hostname = args.get<std::string>("host", "localhost");
  auto master_hostname = args.get<std::string>("host", "localhost");

  auto master_port = args.get<int>("master_port", 6666);
  auto worker_port = args.get<int>("worker_port", 6667);

  worker w(master_hostname, master_port, worker_hostname, worker_port);
  w.setup_worker();

  return 0;
}
