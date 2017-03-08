
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <fstream>
#include <thread>

#include "replica.h"
#include "json.h"
#include "message.h"

using namespace std;

/* Setting up the replica with the provided port and host */
replica::replica(int _port, string _host, int _id, string _config_file)
  : net(_host, _port), learner(4) {

    net.init();

    this->port = _port;
    this->host = _host;
    this->id = _id;
    cur_view_num = 0;

    ifstream config_fs;
    config_fs.open(_config_file);
    // TODO
    // Read from config file
    // Determine number of replicas
    // pass that to learner
  }

/* Start listening on the provided port and host */
void replica::start() {

  while (true) {
    Message *msg = net.recv_from();
    thread t(&replica::handle_msg, this, msg);
    t.detach();
  }
}

/* Handle the given message */
void replica::handle_msg(Message *message) {
  return;

  Message reply;
  switch (message->msg_type) {
    case MessageType::NO_ACTION:
      // do nothing in this case
      break;
    case MessageType::START_PREPARE:
      if(cur_view_num % tot_replicas == my_id)
        reply = proposer.start_prepare(message->prop_number);
      else
        cur_view_num += 1;
      break;
    case MessageType::PREPARE:
      reply = acceptor.prepare(message->prop_number);
      break;
    case MessageType::PREPARE_ACCEPT:
      reply = proposer.prepare_accept(message->n_a, message->value);
      break;
    case MessageType::PREPARE_REJECT:
      reply = proposer.prepare_reject(message->n_p);
      break;
    case MessageType::PROPOSE:
      reply = acceptor.propose(message->prop_number, message->value);
      break;
    case MessageType::PROPOSE_ACCEPT:
      reply = proposer.propose_accept(message->prop_number);
      break;
    case MessageType::PROPOSE_REJECT:
      reply = proposer.propose_reject(message->n_p);
      break;
    case MessageType::BRDCST_LEARNERS:
      reply = learner.update_vote(message->n_a, message->value);
      break;
  }
  delete(message);
  // Send msg if not type noaction

  // If client_req msg
  // skip

  // Incoming: prepare
  // Arguments: n
  // Acting as: Acceptor
  // Check if you've seen an n higher than this one
  // Outgoing: a prepare_accept
  //    OR prepare_reject

  // Incoming: prepare_accept
  // Arguments: n_a, v_a (could be null)
  // Acting as: Proposer
  // Increment the count of # of prepare accepts you have gotten
  // Outgoing: If this is more than a majority, send propose_value(n, v)

  // Incoming: prepare_reject
  // Arguments: n_p
  // Acting as: Proposer
  // If we got a higher n value, update our internal storage with this new value
  // Outgoing:

  // Incoming: propose
  // Arguments: n, v
  // Acting as: Acceptor
  // Outgoing: propose_accept(n) + broadcast_to_learners(n, v),
  //    OR propose_reject(n)

  // Incoming: propose_accept
  // Arguments: n
  // Acting as: Proposer
  // Outgoing:

  // Incoming: propose_reject
  // Arguments: n_p
  // Acting as: Proposer

  // Incoming: broadcast_to_learners
  // Arguments: n, v
  // Acting as: Learner
  // Outgoing: potenial client response


}
