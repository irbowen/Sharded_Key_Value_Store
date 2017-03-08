
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
replica::replica(int _port, string _host, int _id, string _config_file) :
  port(_port), host(_host), id(_id), cur_view_num(0), net(_port, _host)
{
  string h, p, i;
  ifstream config_fs(_config_file);
  while (config_fs >> h >> p >> i) {
    node n;
    n.host = h;
    n.port = stoi(p);
    replicas.push_back(n);
    num_replicas++;
  }
  learner.init(num_replicas);
  proposer.init(replicas);
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
  Message reply;
  cout << "Msg in handle_msg: " << message->serialize() << endl;
  switch (message->msg_type) {
    case MessageType::NO_ACTION:
      // do nothing in this case
      break;
    case MessageType::START_PREPARE:
      // If we are the primary, then we should sent a
      // start prepare message to all other replicas
      if (cur_view_num % num_replicas == id) {
        reply = proposer.start_prepare(message->prop_number);
      }
      // Otherwise, we need to increment our viewnum and assert it
      // is equal to the viewnum sent in the client msg
      else {
        cur_view_num += 1;
      }
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
  if (reply.msg_type != MessageType::NO_ACTION) {
    cout << "Sending out msg: " << reply.serialize() << endl;
    net.sendto(&reply);
  }
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
