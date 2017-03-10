#include "client_lib.h"
#include <cstring>

using namespace std;

int main(){
  client_lib c_lib(6005, "127.0.0.1");

  vector<string> messages = {"Client one chat one", "Client one chat two"};
  for (auto &message : messages){
    cout << "Trying to add " << message << endl;
    c_lib.add_chat_message(message);
  }
}
