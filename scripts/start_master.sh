#!/bin/bash

echo -e "\nStarting up the master..."
./bin/kv_master \
    --port 7000 \
    --host 127.0.0.1 \
    --config configs/master_config.txt &
sleep 1
echo -e "\nMaster is up on port 7000... "
