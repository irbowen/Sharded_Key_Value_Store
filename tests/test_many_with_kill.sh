#!/bin/bash

echo -e "\nRemove old log files..."
touch log_0.txt
rm log_*.txt

# Start up 2f+1 replicas, where f=1
BASE_PORT=8000

echo -e "\nStart up paxos servers..."

for i in {0..4..1}; do
    ./bin/paxos_server \
        --port "$((BASE_PORT + i))" \
        --host 127.0.0.1 \
        --config configs/config5.txt \
        --id "$i" \
        --holes configs/no_holes.txt &
done

# Let them start up
echo -e "\nSleep 5s for the paxos system to start up..."
sleep 5s

echo -e "\nWe are starting up one client..."
./bin/chat_client_3.out &

echo -e "\nKilling the following process...."
ps aux | grep paxos_server | grep "id 0"
ps aux | grep paxos_server | grep "id 0" | awk '{print $2}' | sed -n '1p' | xargs kill

# Make concurrent requests to the paxos system
echo -e "\nStart up a client after failure..."
./bin/chat_client_4.out &

echo -e "\nKilling the following process...."
ps aux | grep paxos_server | grep "id 1"
ps aux | grep paxos_server | grep "id 1" | awk '{print $2}' | sed -n '1p' | xargs kill

echo -e "\nStart up a client after failure..."
./bin/chat_client_5.out

echo -e "\nCheck the log files!"
sleep 5s

echo -e "\nThe log file diffs!"
echo -e "\n########################"
diff log_2.txt log_3.txt
diff log_2.txt log_4.txt
echo -e "\n########################"
echo "Diff ends here. If you see nothing, it worked!"

ps aux | grep paxos_server | awk '{print $2}' | xargs kill
ps aux | grep chat_client | awk '{print $2}' | xargs kill




