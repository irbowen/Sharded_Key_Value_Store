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

# Make concurrent requests to the paxos system
echo -e "\nStart up clients..."
./bin/chat_client_1.out &
./bin/chat_client_2.out &

echo -e "\nCheck the log files!"
sleep 5

echo -e "The log file diffs..."
diff log_0.txt log_1.txt
diff log_0.txt log_2.txt
echo -e "\n\t...Diff ends here. If you see nothing, it worked!"

echo -e "\nHere is the contents of log_0.txt"
cat log_0.txt

ps aux | grep chat_client_ | awk '{print $2}' | xargs kill
ps aux | grep paxos_server | awk '{print $2}' | xargs kill
