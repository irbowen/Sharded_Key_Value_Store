#!/bin/bash

echo -e "\nRemove old log files..."
touch log_0.txt
rm log_*.txt

# Start up 2f+1 replicas, where f=1
BASE_PORT=8000

echo -e "\nStart up paxos servers..."

for i in {0..2..1}; do
    ./bin/paxos_server \
        --port "$((BASE_PORT + i))" \
        --host 127.0.0.1 \
        --config configs/config5.txt \
        --id "$i" \
        --holes configs/no_holes.txt &
done

# Let them start up
sleep 3s

# Make concurrent requests to the paxos system
echo "Start up clients..."
./bin/chat_client_3.out &
./bin/chat_client_4.out &

sleep 10s

echo -e "\nThe log file diffs!"
echo -e "\n########################"
diff log_0.txt log_1.txt
diff log_0.txt log_2.txt
echo -e "\n########################"
echo "Diff ends here. If you see nothing, it worked!"

ps aux | grep paxos_server | awk '{print $2}' | xargs kill 
ps aux | grep chat_client_ | awk '{print $2}' | xargs kill 
