#!/bin/bash

echo -e "\nKilling the following process...."
ps aux | grep paxos_server | grep "id 0"
ps aux | grep paxos_server | grep "id 0" | awk '{print $2}' | xargs kill
