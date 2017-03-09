ps aux | grep paxos_server | awk '{print $2}' | xargs kill 
