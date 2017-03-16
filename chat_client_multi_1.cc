#include "client_lib.h"
#include <cstring>

using namespace std;

int main(){
  client_lib c_lib(6001, "35.2.210.92", "multi_config.txt");

  vector<string> messages = {"Client one chat one", "Client one chat two"};
  for (auto &message : messages){
    cout << "Trying to add " << message << endl;
    c_lib.add_chat_message(message);
  }
}
