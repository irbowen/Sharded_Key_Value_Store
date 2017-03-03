//C++
#include <string>
#include <iostream>
//Project
#include "flags.h"
#include "server.h"

int main(int argc, char* argv[]) {
  const flags::args args(argc, argv);

  auto help = args.get<bool>("help", false);
  if (help) {
    server::print_help_and_exit();
  }

  auto host = args.get<std::string>("host", "localhost");

  auto port = args.get<int>("port");
  if (!port) {
    std::cout << "No port supplied. :(" << std::endl;
    server::print_help_and_exit();
    return -1;
  }

  std::cout << "This platform can support " << std::thread::hardware_concurrency() << " threads are the same time\n";
  server s;
  // This runs in a seperate thread in the server
  s.setup_server_for_requests(*port, host);

  // Wait until we hear from the setup thread that we are ready
  std::cout << "Get's here, so thats cool\n";

  // Don't create worker until master is ready to listen for requests
  // Use condition variable to wait until we get some msg

}
