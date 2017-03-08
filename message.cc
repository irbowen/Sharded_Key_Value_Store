
#include "message.h"

using namespace std;

std::string Message::serialize() {
  char div_char = ':';
  ostringstream oss;;

  oss << msg_type;
  oss << div_char;

  oss << n_a;
  oss << div_char;

  oss << n_p;
  oss << div_char;

  oss << prop_number;
  oss << div_char;

  oss << view_num;
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

  vector<string> array;
  size_t pos = 0, found;
  while ((found = in.find_first_of(':', pos)) != string::npos) {
    array.push_back(in.substr(pos, found - pos));
    pos = found + 1;
  }
  array.push_back(in.substr(pos));

  int msg_int = stoi(array.at(0));
  msg_type = static_cast<MessageType>(msg_int);
  n_a = stoi(array.at(1));
  n_p = stoi(array.at(2));
  prop_number = stoi(array.at(3));
  view_num = stoi(array.at(4));
  value = array.at(5);
  sender.port = stoi(array.at(6));
  sender.host = array.at(7);
  int num_recv = stoi(array.at(8));
  int start = 9;
  // cout << "Num recv: " << num_recv << endl;
  for (int k = 0; k < num_recv; k++) {
    int i = 2 * k;
    // cout << "at: " << array.at(start + i) << " " << array.at(start + 1 + i) << endl;
    node r;
    r.port = stoi(array.at(i + start));
    r.host = array.at(i + 1 + start);
    receivers.push_back(r);
  }
  // cout << "Num recv: " << num_recv << endl;
}
