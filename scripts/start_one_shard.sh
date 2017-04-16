#!/bin/bash


# Start up 2f+1 replicas, where f=1
REPLICAS=(0 1 2)
BASE_PORT=8000
echo -e "\nStart up shard 0 key/value/paxos replicas ..."

for i in {0..2..1}; do
    ./bin/paxos_server \
        --port "$((BASE_PORT + i))" \
        --host 127.0.0.1 \
        --config configs/shard_0.txt \
        --id "$i" \
        --holes configs/no_holes.txt &
done

