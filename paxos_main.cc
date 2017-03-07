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
  while (iarg != -1) {
    iarg = getopt_long(argc, argv, "h:p:i:c:", longopts, &index);
    switch (iarg) {
      case 'h':
        // TODO
        break;
      case 'p':
        // TODO
        break;
      case '?':
        std::cout << "Argument for -h and -p required\n";
        exit(1);
    }
  }

  replica r(6000, "127.0.0.1", 0, "config.txt");
  r.start();
  std::cout << "Get's here, so thats cool\n";
}