#!/bin/bash

echo -e "\nKilling the following process...."
ps aux | grep paxos_server | grep "id 1"
ps aux | grep paxos_server | grep "id 1" | awk '{print $2}' | xargs kill
