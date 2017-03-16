ps aux | grep paxos_server | awk '{print $2}' | xargs kill 
ps aux | grep chat_client_ | awk '{print $2}' | xargs kill 
