#include "client_lib.h"
#include <cstring>

using namespace std;

int main(){
  client_lib c_lib;

  //vector<string> messages = {"Hello", "How are you", "Do you like apples?"};
  vector<string> messages = {"EECS 591 test"};
  for (auto &message : messages){
    cout << "Trying to add " << message << endl;
    c_lib.add_chat_message(message);
  }
}
