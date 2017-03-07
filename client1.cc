#include "client_lib.h"
#include <cstring>
using namespace std;
int main(){
	client_lib c_lib;

	vector<string> messages = {"Hello", "How are you", "Do you like apples?"};

    for(auto &message : messages){
        c_lib.add_chat_message(message);
    }
}
