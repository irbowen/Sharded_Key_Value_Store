#!/usr/bin/python3

f = input('How many failures to tolerate per shard? ')
n = input('How many shards? ')
p = input('Starting port (try 8000)? ')

startup = ''
master_config_data = '127.0.0.1 7000\n%s\n' % f
for i in range(0, int(n)):
    shard_port = int(p) + 100 * i
    shard_config = 'configs/shard_%d.txt' % i
    master_config_data += shard_config + '\n'
    shard_data = ''
    t = 2 * int(f) + 1
    for k in range(0, t):
        start_str = "./bin/paxos_server --port %d --host 127.0.0.1 --config %s --id %d &\n" % (shard_port + k, shard_config, k)
        startup += start_str
        shard_data += "127.0.0.1 %d %d\n" % (shard_port + k, k)
    f_ = open(shard_config, 'w')
    f_.write(shard_data)

f_ = open('scripts/start_replicas.sh', 'w')
f_.write(startup)
f_ = open('configs/master_config.txt', 'w')
f_.write(master_config_data)
