# Distributed Chat Service (using paxos)

irbowen and pranavr

Our paxos implmentation is written in c++14. You can git clone the repo, cd in the dir, and type `make`.
This will build the binary for the paxos replicas and the binary for many clients that can be used for testing.

Each binary requires several command line arguments, as well as a configuration file containing the command line arguments of all other replicas in the system. Because `f` can vary, we have a script to generate the startup script based on f.

Run `python3 setup_scripts.py -p 8000 -f 1`, with whatever ports and f values you would like to start with.

This will create a `start_paxos.sh` file that looks like this

```bash
./paxos_server --port 8000 --host 127.0.0.1 --config config.txt --id 0 --holes holes.txt &
./paxos_server --port 8001 --host 127.0.0.1 --config config.txt --id 1 --holes holes.txt &
./paxos_server --port 8002 --host 127.0.0.1 --config config.txt --id 2 --holes holes.txt &
```
--port: The port the replica will listen on
--host: The host ip address. We used 127.0.0.1 to make testing easier, but this can be changed
--config: The config file containing the host and port info for all replicas. This file must be the same and available on all systems where paxos is reunning.
--id: The id number of this replica, used to determine if we are the primary
--holes: The file that contains the seqnums that we should skip to simulate msg loss

And a config.txt that looks like this

```bash
127.0.0.1 8000 0
127.0.0.1 8001 1
127.0.0.1 8002 2
```

If you'd prefer to not to deal with all of these command line arguments, we have provided some bash scripts that test many aspects of our system.

- `test_basic.sh` - This starts up 3 replicas, and a few clients, and makes sure that the logs are consistent.
- `test_holes.sh` - This starts up 3 replicas, and a few clients, and makes sure that the logs are consistent when the replica is instructed to skip a few seqnums to simulate msg failure.
- `test_kill_one.sh` - This script starts up 3 replicas, launches a client, and then kills one of the replicas. It launches some more clients, and makes sure that the logs are still consistent for the replicas that are alive.
- `test_kill_f.sh` - This sript starts up 5 replicas, launches a client, and then kills one replica. Then, it launches another client.  Then, it kills ANOTHER REPLICA, and then CREATES ANOTHER CLIENT. It then checks that all of the log files for the 3 replicas still up are consistent
- `test_many.sh` - This starts up 3 replicas, and then start 2 clients. These aren't your ordinary clients though. They bombard paxos with lots of mgs, making sure our system can do work.
- `test_many_with_kill.sh` - All the jass of the last one, except that it also kill paxos replicas! It launches 5 replicas, a few nasy clients that hit the system hard, and it kills 2 of the 5 replicas.
