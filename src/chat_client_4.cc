#include "../headers/client_lib.h"
#include <cstring>

using namespace std;

int main(){
  client_lib c_lib(6004, "127.0.0.1", "config.txt");

  vector<string> messages;
  for (int i = 0; i < 100; i++) {
    messages.push_back("C4M" + to_string(i));
  }

  for (auto &message : messages){
    cout << "Trying to add " << message << endl;
    c_lib.add_chat_message(message);
  }
}
