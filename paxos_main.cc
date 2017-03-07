//C++
#include <string>
#include <iostream>
#include <getopt.h>

//Project
#include "replica.h"

#define no_argument 0
#define required_argument 1
#define optional_argument 2

int main(int argc, char* argv[]) {

  const struct option longopts[] = {
    {"host", required_argument, 0, 'h'},
    {"port", required_argument, 0, 'p'},
    {"id", required_argument, 0, 'i'},
    {"config", required_argument, 0, 'c'},
    {0,0,0,0},
  };
  int index;
  int iarg = 0;
  string host, config;
  int port, id;
  while (iarg != -1) {
    iarg = getopt_long(argc, argv, "h:p:i:c:", longopts, &index);
    switch (iarg) {
      case 'h':
        host = optarg;
        break;
      case 'p':
        port = atoi(optarg);
        break;
      case 'i':
        id = atoi(optarg);
        break;
      case 'c':
        config = optarg;
        break;
      case '?':
        std::cout << "Argument for -h and -p required\n";
        exit(1);
    }
  }

  replica r(port, host, id, config);
  r.start();
  std::cout << "Get's here, so thats cool\n";
}
