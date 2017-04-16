
#include "../headers/client_lib.h"
#include <cstring>
#include <thread>
#include <chrono>

using namespace std;

int main(){
    client_lib c_lib(7000, "127.0.0.1", 5000, "127.0.0.1");

    string str;
    cout << "put, get, delete, add or exit?\n";
    while (getline(cin, str)) {
        if (str == "put") {
            string key, value;
            cout << "Enter key: " << endl;
            getline(cin, key);
            cout << "Enter value: ";
            getline(cin, value);
            c_lib.put(key, value);
            cout << "Key-Value pair added!" << endl;
        }
        else if (str == "get") {
            string key;
            cout << "Enter key: ";
            getline(cin, key);
            cout << c_lib.get(key) << endl;
        }
        else if (str == "delete") {
            string key;
            cout << "Enter key: ";
            getline(cin, key);
            c_lib.delete_key(key);
            cout << "Deleted key";
        }
        else if(str == "add") {
            cout << "Adding shard\n";
            c_lib.add_shard();
        }
        else if (str == "exit") {
            break;
        }
        cout << "put, get, delete, add or exit\n";
    }

    return 0;
}
