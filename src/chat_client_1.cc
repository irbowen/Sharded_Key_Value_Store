#include "../headers/client_lib.h"
#include <cstring>
#include <thread>
#include <chrono>

using namespace std;

int main(){
  client_lib c_lib(6001, "127.0.0.1", "configs/config3.txt");

  vector<string> messages = {"Client one chat one", "Client one chat two"};
  for (auto &message : messages){
    cout << "Trying to add " << message << endl;
    c_lib.add_chat_message(message);
    std::this_thread::sleep_for (std::chrono::seconds(1));

  }
}
