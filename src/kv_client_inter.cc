
#include "../headers/client_lib.h"
#include <cstring>
#include <thread>
#include <chrono>

using namespace std;

int main() {
    client_lib c_lib(7000, "127.0.0.1", 5000, "127.0.0.1");

    string str;
    cout << "put, get, delete, add or exit?\n";
    while (getline(cin, str)) {
        string key, column, value, config;
        if (str == "put") {
            cout << "Enter key: " << endl;
            getline(cin, key);
            cout << "Enter column: " << endl;
            getline(cin, column);
            cout << "Enter value: ";
            getline(cin, value);
            c_lib.put(key, column, value);
            cout << "Key-Value pair added!" << endl;
        }
        else if (str == "get") {
            cout << "Enter key: ";
            getline(cin, key);
            cout << "Enter column: " << endl;
            getline(cin, column);
            cout << c_lib.get(key, column) << endl;
        }
        else if (str == "delete") {
            cout << "Enter key: ";
            getline(cin, key);
            cout << "Enter column: " << endl;
            getline(cin, column);
            c_lib.delete_key(key, column);
            cout << "Deleted key\n";
        }
        else if(str == "add") {
            cout << "Enter config file\n";
            getline(cin, config);
            c_lib.add_shard(config);
        }
        else if (str == "exit") {
            break;
        }
        cout << "put, get, delete, add or exit\n";
    }

    return 0;
}
