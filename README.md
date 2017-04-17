# A Distributed, Sharded Key-Value Store
## Written in C++, backed by Paxos

Isaac Bowen(irbowen) & Pranav Ramarao(pranavr)

Our paxos implmentation is written in c++14.
You can get started with the following

```bash
git@github.com:irbowen/Sharded_Key_Value_Store.git
cd Sharded_Key_Value_Store
mkdir bin
mkdir objs

# generate config files based on #shards and #tolerated failures
python3 make_scripts.py

# Make sure we can run the scripts
chmod +x scripts/*
chmod +x run.sh

# Run the whole setup!
./run.sh

```
This will build the binares for the key-value/paxos replicas, the master, then launch the replicas
and master in the configuration specified by the make\_scripts.py file. It will also launch an interactive client
that can be used to test the service.


You can see all the running processes with

```bash
./scripts/show_all.sh
```

