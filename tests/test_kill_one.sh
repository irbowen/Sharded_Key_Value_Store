
ps aux | grep chat_client_ | awk '{print $2}' | xargs kill
ps aux | grep paxos_server | awk '{print $2}' | xargs kill

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
        --config configs/config3.txt \
        --id "$i" \
        --holes configs/no_holes.txt &
done

# Let them start up
sleep 3s

echo -e "We are starting up one client..."
./bin/chat_client_1.out &

echo -e "\nKilling the following process...."
ps aux | grep paxos_server | grep "id 0"
ps aux | grep paxos_server | grep "id 0" | awk '{print $2}' | sed -n '1p' | xargs kill

echo -e "\nHere are all of the processes now..."
ps aux | grep paxos_server

# Make concurrent requests to the paxos system
echo -e "\nStart up clients..."
./bin/chat_client_2.out &

echo -e "\nCheck the log files!"
sleep 5s

echo -e "The log file diffs..."
echo -e "\n########################"
diff log_1.txt log_2.txt
echo -e "\n########################"
echo "Diff ends here. If you see nothing, it worked!"

echo -e "\nHere is the contents of log_1.txt (because log_0.txt is out of date - that one is dead"
cat log_1.txt

ps aux | grep paxos_server | awk '{print $2}' | xargs kill 
ps aux | grep chat_client_ | awk '{print $2}' | xargs kill 
