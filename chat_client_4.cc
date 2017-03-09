#include "client_lib.h"
#include <cstring>

using namespace std;

int main(){
  client_lib c_lib(6003, "127.0.0.1");

  vector<string> messages = {"C4 msg 1", "C4 msg 2", "C4 msg 3"};
  for (auto &message : messages){
    cout << "Trying to add " << message << endl;
    c_lib.add_chat_message(message);
  }
}
