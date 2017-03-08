
#include "acceptor.h"

Message* Acceptor::prepare(int proposal_number) {
  Message *msg = new Message;
  if (proposal_number < n_p) {
    msg->msg_type = MessageType::PREPARE_REJECT;
    msg->n_p = n_p;
  } else {
    msg->msg_type = MessageType::PREPARE_ACCEPT;
    msg->n_a = n_a;
    msg->value = value;
  }
  return msg;
}

Message* Acceptor::propose(int proposal_number, std::string new_value){
  Message *msg = new Message;
  if (proposal_number < n_p) {
    msg->msg_type = MessageType::PROPOSE_REJECT;
    msg->n_p = n_p;
  } else {
    n_a = n_p = proposal_number;
    value = new_value;
    msg->msg_type = MessageType::BRDCST_LEARNERS;
    msg->value = new_value;
    msg->n_a = n_a;
  }
  return msg;
}
