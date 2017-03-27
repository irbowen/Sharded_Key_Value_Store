	
echo -e "\nKilling all paxos_server processes..."
ps aux | grep paxos_server | awk '{print $2}' | xargs kill 

echo -e "\nRemove old log files..."
touch log_0.txt log_1.txt log_2.txt log_3.txt log_4.txt
rm log_*.txt

# Start up 2f+1 replicas, where f =1
echo -e "\nStart up paxos servers..."
./paxos_server --port 8000 --host 127.0.0.1 --config config5.txt --id 0 --holes no_holes.txt &
./paxos_server --port 8001 --host 127.0.0.1 --config config5.txt --id 1 --holes no_holes.txt &
./paxos_server --port 8002 --host 127.0.0.1 --config config5.txt --id 2 --holes no_holes.txt &
./paxos_server --port 8003 --host 127.0.0.1 --config config5.txt --id 3 --holes no_holes.txt &
./paxos_server --port 8004 --host 127.0.0.1 --config config5.txt --id 4 --holes no_holes.txt &

# Let them start up
echo -e "\nSleep 5s for the paxos system to start up..."
sleep 5s

echo -e "\nWe are starting up one client..."
./chat_client_1.out &

echo -e "\nHere are all of the processes, we are going to kill on of them later..."
ps aux | grep paxos_server
echo -e "\nKilling the following process...."
ps aux | grep paxos_server | grep "id 0"
ps aux | grep paxos_server | grep "id 0" | awk '{print $2}' | sed -n '1p' | xargs kill
echo -e "\nHere are all of the processes now..."
ps aux | grep paxos_server

# Make concurrent requests to the paxos system
echo -e "\nStart up a client after failure..."
./chat_client_2.out &

echo -e "\nHere are all of the processes, we are going to kill on of them later..."
ps aux | grep paxos_server
echo -e "\nKilling the following process...."
ps aux | grep paxos_server | grep "id 1"
ps aux | grep paxos_server | grep "id 1" | awk '{print $2}' | sed -n '1p' | xargs kill
echo -e "\nHere are all of the processes now..."
ps aux | grep paxos_server

echo -e "\nStart up a client after failure..."
./chat_client_5.out &

echo -e "\nCheck the log files!"
sleep 5s

echo -e "\nThe log file diffs!"
diff log_2.txt log_3.txt
diff log_2.txt log_4.txt
echo "Diff ends here. If you see nothing, it worked!"

echo -e "\nHere is the contents of log_2.txt"
cat log_2.txt

ps aux | grep paxos_server | awk '{print $2}' | xargs kill



