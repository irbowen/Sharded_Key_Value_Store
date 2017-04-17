
#include "../headers/client_lib.h"
#include <cstring>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
using namespace std;
template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}


int main(){
    client_lib c_lib(7000, "127.0.0.1", 5000, "127.0.0.1");
    string str;
    cout << "Usage: " << endl;
    cout << ">> put map reduce\n";
    cout << ">> key-value added\n";
    cout << ">> get map\n";
    cout << ">> reduce\n";
    cout << ">> add configs/shard_1.txt\n";
    cout << ">> shard added\n";
    cout << ">> delete map\n";
    cout << ">> map deleted\n";
    cout << ">> exit\n";
    cout << "\n\n----- KEY VALUE STORE INTERACTION BEGINS -----\n\n";
    cout << ">> ";
    while (getline(cin, str)) {
        vector<string> items = split(str, ' ');
        string input = items.size() > 0 ? items[0] : "";
        string key = items.size() > 1 ? items[1] : "";
        string value = items.size() > 2 ? items[2] : "";
        if (input == "get"){
            cout << ">> " << c_lib.get(key) << endl;
        }
        else if(input == "put"){
            c_lib.put(key, value);
            cout <<">> key-value added\n";
        }
        else if(input == "delete"){
            c_lib.delete_key(key);
            cout <<">> " << key << " deleted" << endl;
        }
        else if(input == "add"){
            c_lib.add_shard(key);
            cout << ">> shard added" << endl;
        } 
        else if (input == "exit") {
            cout << ">> Exiting ...";
            return 0;
        }
        cout << ">> ";
        //cout << "put, get, delete, add or exit\n";
    }

    return 0;
}
