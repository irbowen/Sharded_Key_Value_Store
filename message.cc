#include <sstream>
#include "message.h"
std::string Message::serialize() {
  char div_char = ':';
  std::ostringstream oss;

  oss << msg_type;
  oss << div_char;

  oss << n_a;
  oss << div_char;

  oss << n_p;
  oss << div_char;

  oss << prop_number;
  oss << div_char;

  oss << value;
  oss << div_char;

  oss << sender.port;
  oss << div_char;
  oss << sender.host;
  oss << div_char;

  oss << receivers.size();
  oss << div_char;
  for (auto& r : receivers) {
    oss << r.port;
    oss << div_char;
    oss << r.host;
    oss << div_char;
  }
  return oss.str();
}

void Message::deserialize(std::string in) {

  std::vector<std::string> array;
  size_t pos = 0, found;
  while ((found = in.find_first_of(':', pos)) != std::string::npos) {
    array.push_back(in.substr(pos, found - pos));
    pos = found + 1;
  }
  array.push_back(in.substr(pos));

  msg_type = (MessageType)stoi(array.at(0));
  n_a = stoi(array.at(1));
  n_p = stoi(array.at(2));
  prop_number = stoi(array.at(3));
  value = array.at(4);
  sender.port = stoi(array.at(5));
  sender.host = array.at(6);
  int num_recv = stoi(array.at(7));
  for (int i = 0; i < num_recv; i++) {
    node r;
    r.port = stoi(array.at(8 + i));
    r.host = array.at(9 + i);
    receivers.push_back(r);
  }
}
