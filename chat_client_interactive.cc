#include "client_lib.h"
#include <cstring>

using namespace std;

int main(){
  client_lib c_lib(6003, "127.0.0.1", "config.txt");

  string str;
  while (getline(cin, str)) {
    c_lib.add_chat_message(str);
  }
}
