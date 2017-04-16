
#include "../headers/client_lib.h"
#include <cstring>
#include <thread>
#include <chrono>

using namespace std;

int main(){
    client_lib c_lib(7000, "127.0.0.1", 5000, "127.0.0.1");

    string str;
    while (getline(cin, str)) {
        cout << "put, get, or delete?\n";
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
        else if (str == "delete") {
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
