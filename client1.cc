#include "client_lib.h"
#include <cstring>
using namespace std;
int main(){
	client_lib c_lib;

	vector<string> messages = {"Hello", "How are you", "Do you like apples?"};

	for_each(messages.begin(), messages.end(), [&](const string &message){
		c_lib.add_chat_message(message);
	});
}
