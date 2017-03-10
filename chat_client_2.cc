#include "client_lib.h"
#include <cstring>

using namespace std;

int main(){
  client_lib c_lib(6002, "127.0.0.1", "config5.txt");

  vector<string> messages = {"C2M1", "C2M2", "C2M3"};
  for (auto &message : messages){
    cout << "Trying to add " << message << endl;
    c_lib.add_chat_message(message);
  }
}
