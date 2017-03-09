#! /bin/bash

echo "Here are all of the processes..."
ps aux | grep paxos_server

echo "Killing the following process...."
ps aux | grep paxos_server | grep "id 0"
ps aux | grep paxos_server | grep "id 0" | awk '{print $2}' | sed -n '1p' | xargs kill

echo "Here are all of the processes now..."
ps aux | grep paxos_server
