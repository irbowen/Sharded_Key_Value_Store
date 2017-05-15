//C++
#include <string>
#include <iostream>
#include <getopt.h>

//Project
#include "../headers/replica.h"
#include "../headers/environment.h"


#define no_argument 0
#define required_argument 1
#define optional_argument 2

int main(int argc, char* argv[]) {

    const struct option longopts[] = {
        {"host", required_argument, 0, 'h'},
        {"port", required_argument, 0, 'p'},
        {"id", required_argument, 0, 'i'},
        {"config", required_argument, 0, 'c'},
        {"holes", required_argument, 0, 'g'},
        {0,0,0,0},
    };
    int index;
    int iarg = 0;
    string host, config, holes = "";
    int port = -1, id = -1;
    while (iarg != -1) {
        iarg = getopt_long(argc, argv, "h:p:i:c:g:", longopts, &index);
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
            case 'g':
                if(optarg != NULL)
                    holes = optarg;
                break;
            case '?':
                COUT << "Argument for -h and -p required\n";
                exit(1);
        }
    }
    assert(port != -1 && id != -1);
    // Create the environment, and then pass that to the replica
    network net(port, host);
    Environment env(port, host, id, config, &net, false);
    Replica r(&env, holes);
    r.start();
}
