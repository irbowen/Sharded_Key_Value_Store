#include <string>
#include <iostream>
#include <getopt.h>

#include "../headers/master.h"

#define no_argument 0
#define required_argument 1
#define optional_argument 2

int main(int argc, char* argv[]) {
    
    const struct option longopts[] = {
        {"host", required_argument, 0, 'h'},
        {"port", required_argument, 0, 'p'},
        {"config", required_argument, 0, 'c'},
        {0,0,0,0},
    };
    int index;
    int iarg = 0;
    string host, config = "";
    int port = -1, id = -1;
    while (iarg != -1) {
        iarg = getopt_long(argc, argv, "h:p:c:", longopts, &index);
        switch (iarg) {
            case 'h':
                host = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'c':
                config = optarg;
                break;
            case '?':
                cout << "Argument for -h and -p and -c required\n";
                exit(1);
        }
    }
    assert(port != -1);
    Master m(port, host, config);
    m.recv();
}
