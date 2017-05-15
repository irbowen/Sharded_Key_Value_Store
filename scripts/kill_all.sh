#!/bin/bash

echo -e "We are killing all the paxos servers and the chat clients\n"

ps aux | grep paxos_server | awk '{print $2}' | xargs kill
ps aux | grep replica_exec | awk '{print $2}' | xargs kill
ps aux | grep chat_client_ | awk '{print $2}' | xargs kill
ps aux | grep kv_client_ | awk '{print $2}' | xargs kill
ps aux | grep kv_master | awk '{print $2}' | xargs kill
