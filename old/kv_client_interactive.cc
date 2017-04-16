
#include "../headers/client_lib.h"
#include <cstring>
#include <thread>
#include <chrono>

using namespace std;

int main(){
    client_lib c_lib(6001, "127.0.0.1", "configs/config3.txt");

    string str;
    while (getline(cin, str)) {
        cout << "put or get?\n";
        if (str == "put") {
            string key, value;
            cout << "Key?:" << endl;
            getline(cin, key);
            cout << "Value?:";
            getline(cin, value);
            cout << "Working..." << endl;
            c_lib.put(key, value);
            cout << "Done!" << endl;
        }
        else if (str == "get") {
            string key;
            cout << "Key?:";
            cout << c_lib.get(key) << endl;
        }
        else if (str == "break") {
            break;
        }
    }

    return 0;
}
