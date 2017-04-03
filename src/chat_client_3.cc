#include "../headers/client_lib.h"
#include <cstring>

using namespace std;

int main(){
  client_lib c_lib(6001, "127.0.0.1", "configs/config5.txt");

  vector<string> messages;
  for (int i = 0; i < 100; i++) {
    messages.push_back("C3M" + to_string(i));
  }

  for (auto &message : messages){
    cout << "Trying to add " << message << endl;
    c_lib.add_chat_message(message);
  }
}
