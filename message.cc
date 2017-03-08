
#include "message.h"

using namespace std;

std::string Message::serialize() {
  char div_char = ' ';
  ostringstream oss;;

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

  for (auto& r : receivers) {
    oss << r.port;
    oss << div_char;
    oss << r.host;
    oss << div_char;
  }

  std::cout << oss.str();
  return oss.str();
}
