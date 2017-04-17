./scripts/kill_all.sh;
make clean; make;
./scripts/start_master.sh &
./scripts/start_replicas.sh &
sleep 1
./bin/kv_client_fast.out
