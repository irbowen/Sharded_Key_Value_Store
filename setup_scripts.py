#! /usr/bin/env python3


import click

DEFAULT_PORT_NUM=9000
DEFAULT_REPLICA_NUM=4

BINARY_NAME="paxos_server"
CONFIG_FILE="config.txt"

@click.command()
@click.option("--fault_tolerance", "-f", "num_replicas",
    default=DEFAULT_REPLICA_NUM,
    help="The number of replicas that you would like to run, default " + str(DEFAULT_REPLICA_NUM))

@click.option("--port_number", "-p", "port_number",
    default=DEFAULT_PORT_NUM,
    help="The port the master is listening on, default " + str(DEFAULT_PORT_NUM))

def main(num_replicas, port_number):
    # Create startup script
    output_str = ""
    config_file = ""
    for i in range(2 * num_replicas + 1):
        output_str += "./%s --port %s --host %s --config %s --id %d &\n" % (
            BINARY_NAME, (port_number + i), "127.0.0.1", CONFIG_FILE, i)
        config_file += "%s, %s, %d\n" % ("127.0.0.1", (port_number + i), i)
    sf = open('start.sh', 'w')
    sf.write(output_str)
    cf = open('config.txt', 'w')
    cf.write(config_file)

if __name__ == "__main__":
  main()
