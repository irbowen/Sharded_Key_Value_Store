./bin/paxos_server --port 8000 --host 127.0.0.1 --config configs/shard_0.txt --id 0 &
./bin/paxos_server --port 8001 --host 127.0.0.1 --config configs/shard_0.txt --id 1 &
./bin/paxos_server --port 8002 --host 127.0.0.1 --config configs/shard_0.txt --id 2 &
./bin/paxos_server --port 8003 --host 127.0.0.1 --config configs/shard_0.txt --id 3 &
./bin/paxos_server --port 8004 --host 127.0.0.1 --config configs/shard_0.txt --id 4 &
