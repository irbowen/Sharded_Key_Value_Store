./scripts/kill_all.sh;
make clean; make;
./scripts/start_master.sh &
./scripts/start_replicas.sh &
sleep 1
./bin/kv_client_auto_1.out &
./bin/kv_client_auto_2.out &