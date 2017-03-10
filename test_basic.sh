
ps aux | grep paxos_server | awk '{print $2}' | xargs kill 

echo ""
echo "Remove old log files..."
touch log_0.txt log_1.txt log_2.txt
rm log_*.txt

# Start up 2f+1 replicas, where f =1
echo ""
echo "Start up paxos servers..."
./paxos_server --port 8000 --host 127.0.0.1 --config config.txt --id 0 --holes no_holes.txt &
./paxos_server --port 8001 --host 127.0.0.1 --config config.txt --id 1 --holes no_holes.txt &
./paxos_server --port 8002 --host 127.0.0.1 --config config.txt --id 2 --holes no_holes.txt &

# Let them start up
sleep 3s

# Make concurrent requests to the paxos system
echo ""
echo "Start up clients..."
./chat_client_1.out &
./chat_client_2.out &

echo ""
echo "Check the log files!"
sleep 5s

echo ""
echo "The log file diffs::"
diff log_0.txt log_1.txt
diff log_0.txt log_2.txt
echo "::Diff ends here. If you see nothing, it worked!"

echo "Here is the contents of log_0.txt"
cat log_0.txt

ps aux | grep paxos_server | awk '{print $2}' | xargs kill 
