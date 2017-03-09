#! /bin/bash

echo ""
echo "Here are all of the processes..."
ps aux | grep paxos_server

echo ""
echo "Killing processes one at a time..."
for i in {0..1}
do
    echo ""
    echo "Killing the following process...."
    ps aux | grep paxos_server | grep "id $i"
    ps aux | grep paxos_server | grep "id $i" | awk '{print $2}' | sed -n '1p' | xargs kill

    echo ""
    echo "Here are all of the processes now..."
    ps aux | grep paxos_server
    sleep 7s
done

