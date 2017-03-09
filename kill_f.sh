#! /bin/bash

echo "Here are all of the processes..."
ps aux | grep paxos_server

echo "Killing the following process...."
for i in {0..1}
do
    ps aux | grep paxos_server | grep "id $i"
    ps aux | grep paxos_server | grep "id $i" | awk '{print $2}' | sed -n '1p' | xargs kill

    echo "Here are all of the processes now..."
    ps aux | grep paxos_server
    sleep 3s
done

