
#include "../headers/client_lib.h"
#include <cstring>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <unordered_map>
#include <unistd.h>

using namespace std;
template<typename Out>
void split(const std::string &s, char delim, Out result);
std::vector<std::string> split(const std::string &s, char delim);


#define N 50
#define MILLISECONDS 1000
int main(){
    client_lib c_lib(7000, "127.0.0.1", 5001, "127.0.0.1");
    string str = "sharded";
    string val = "keyvalue";

    vector<string> key;
    vector<string> value;
    unordered_map<string, string> map;

    for(int i = 0; i < N; i++){
        key.push_back(str + to_string(i));
        value.push_back(str + to_string(i));
    }
    for(int i = 0; i < N; i++){
        c_lib.put(key[i], value[i]);
        map[key[i]] = value[i];

        if(i % (3*N/4) == 0 && i != 0){
            c_lib.add_shard("configs/shard_2.txt");
        }
        usleep(500 * MILLISECONDS);
        cout << i <<": put from client 2" << endl;
    }
    for(int i = 0; i< N; i ++){
        if(i % 23 == 0){
            // delete some keys
            c_lib.delete_key(key[i]);
            map.erase(key[i]);
        }
        if(i % 41 == 0){
            // modify existing values
            c_lib.put(str + to_string(i), value[i] + to_string(i));
            map[key[i]] = value[i] + to_string(i);
        }
        usleep(500 * MILLISECONDS);
        cout << i <<": delete from client 2" << endl;
    }
    cout << "added all keys\n";

    // test each key one by one
    for(auto i : map){
        cout << "Actual => " << i.first << " : " << i.second << endl;
        cout << "Sharded => " << i.first << " : " << c_lib.get(i.first) << endl;
        assert(c_lib.get(i.first) == i.second);
        cout << endl;
    }
    cout << "all tests passed!";

    return 0;
}
template<typename Out>
void split(const std::string &s, char delim, Out result){
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
