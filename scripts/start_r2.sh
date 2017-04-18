./bin/paxos_server --port 8100 --host 127.0.0.1 --config configs/shard_1.txt --id 0 &
./bin/paxos_server --port 8101 --host 127.0.0.1 --config configs/shard_1.txt --id 1 &
./bin/paxos_server --port 8102 --host 127.0.0.1 --config configs/shard_1.txt --id 2 &
./bin/paxos_server --port 8103 --host 127.0.0.1 --config configs/shard_1.txt --id 3 &
./bin/paxos_server --port 8104 --host 127.0.0.1 --config configs/shard_1.txt --id 4 
