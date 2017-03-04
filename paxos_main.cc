//C++
#include <string>
#include <iostream>
#include <getopt.h>
//Project
#include "paxos.h"

#define no_argument 0
#define required_argument 1
#define optional_argument 2

int main(int argc, char* argv[]) {

  const struct option longopts[] = {
    {"host", required_argument, 0, 'h'},
    {"port", required_argument, 0, 'p'},
    {0,0,0,0},
  };
  int index;
  int iarg = 0;
  while (iarg != -1) {
    iarg = getopt_long(argc, argv, "", longopts, &index);
    switch (iarg) {
      case 'h':
        break;
      case 'p':
        break;
      case '?':
        std::cout << "Argument for -h and -p required\n";
        exit(1);
    }
  }

  std::cout << "This platform can support " << std::thread::hardware_concurrency() << " threads are the same time\n";
  paxos p;
  p.setup_server_for_requests(6666, "localhost");
  std::cout << "Get's here, so thats cool\n";
}
