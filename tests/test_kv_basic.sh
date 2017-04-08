#!/bin/bash

ps aux | grep chat_client_ | awk '{print $2}' | xargs kill
ps aux | grep paxos_server | awk '{print $2}' | xargs kill

echo -e "\nRemove old log files..."
touch log_0.txt
rm log_*.txt

# Start up 2f+1 replicas, where f=1
REPLICAS=(0 1 2)
BASE_PORT=8000

echo -e "\nStart up paxos servers..."

for i in {0..2..1}; do
    ./bin/paxos_server \
        --port "$((BASE_PORT + i))" \
        --host 127.0.0.1 \
        --config configs/config3.txt \
        --id "$i" \
        --holes configs/no_holes.txt &
done

# Let them start up
sleep 3s

echo -e "\nStart up clients..."
./bin/kv_client_1.out &

sleep 5

ps aux | grep chat_client_ | awk '{print $2}' | xargs kill
ps aux | grep paxos_server | awk '{print $2}' | xargs kill
