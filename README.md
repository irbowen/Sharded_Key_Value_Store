# paxos

Our paxos implmentation is written in c++14. You can git clone the repo, cd in the dir, and type `make`.

This will build the binary for the paxos replicas and the binary for many clients that can be used for testing.

Each binary requires several command line arguments, as well as a configuration file containing the command line arguments of all other replicas in the system. Because `f` can vary, we have a script to generate the startup script based on f.

Run `python3 setup_scripts.py -p 8000 -f 1`, with whatever ports and f values you would like to start with.

This will create a start.sh file that looks like this
```bash
./paxos_server --port 8000 --host 127.0.0.1 --config config.txt --id 0 &
./paxos_server --port 8001 --host 127.0.0.1 --config config.txt --id 1 &
./paxos_server --port 8002 --host 127.0.0.1 --config config.txt --id 2 &
```

And a config.txt that looks like this

```bash
127.0.0.1 8000 0
127.0.0.1 8001 1
127.0.0.1 8002 2
```
