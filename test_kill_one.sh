
echo ""
echo "Killing all paxos_server processes..."
ps aux | grep paxos_server | awk '{print $2}' | xargs kill 

echo ""
echo "Remove old log files..."
touch log_0.txt log_1.txt log_2.txt
rm log_*.txt

# Start up 2f+1 replicas, where f =1
echo ""
echo "Start up paxos servers..."
./paxos_server --port 8000 --host 127.0.0.1 --config config.txt --id 0 &
./paxos_server --port 8001 --host 127.0.0.1 --config config.txt --id 1 &
./paxos_server --port 8002 --host 127.0.0.1 --config config.txt --id 2 &

# Let them start up
echo ""
echo "Sleep 3s for the paxos system to start up..."
sleep 3s

echo ""
echo "Here are all of the processes, we are going to kill on of them later..."
ps aux | grep paxos_server

echo ""
echo "We are starting up one client..."
./chat_client_1.out &

echo ""
echo "Killing the following process...."
ps aux | grep paxos_server | grep "id 0"
ps aux | grep paxos_server | grep "id 0" | awk '{print $2}' | sed -n '1p' | xargs kill

echo ""
echo "Here are all of the processes now..."
ps aux | grep paxos_server

# Make concurrent requests to the paxos system
echo ""
echo "Start up a client after failure..."
./chat_client_2.out &

echo ""
echo "Check the log files!"
sleep 5s

echo ""
echo "The log file diffs!"
diff log_1.txt log_2.txt
echo "Diff ends here. If you see nothing, it worked!"

echo "Here is the contents of log_1.txt (because log_0.txt is out of date - that one is dead"
cat log_1.txt

ps aux | grep paxos_server | awk '{print $2}' | xargs kill 
