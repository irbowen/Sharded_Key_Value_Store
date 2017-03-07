#! /usr/bin/env python3


import click

DEFAULT_PORT_NUM=9000
DEFAULT_REPLICA_NUM=4

BINARY_NAME="paxos_server"
CONFIG_FILE="config.txt"

@click.command()
@click.option("--num_replicas", "-n", "num_replicas",
    default=DEFAULT_REPLICA_NUM,
    help="The number of replicas that you would like to run, default " + str(DEFAULT_REPLICA_NUM))

@click.option("--port_number", "-p", "port_number",
    default=DEFAULT_PORT_NUM,
    help="The port the master is listening on, default " + str(DEFAULT_PORT_NUM))

def main(num_replicas, port_number):
    # Create
    output_str = ""
    for i in range(num_replicas):
        output_str += "./%s --port %s --host %s --config %s --id %d\n" % (
            BINARY_NAME, (port_number + i), "127.0.0.1", CONFIG_FILE, i)
    print(output_str)
    f = open('start.sh', 'w')
    f.write(output_str)

if __name__ == "__main__":
  main()
