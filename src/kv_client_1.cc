
#include "../headers/client_lib.h"
#include <cstring>
#include <thread>
#include <chrono>

using namespace std;

int main(){
    client_lib c_lib(6001, "127.0.0.1", "configs/config3.txt");

    c_lib.put("money", "is power");
    assert(c_lib.get("money") == "is power");
    std::this_thread::sleep_for (std::chrono::milliseconds(500));

    c_lib.put("greed", "is good");
    assert(c_lib.get("greed") == "is good");
    std::this_thread::sleep_for (std::chrono::milliseconds(500));

    c_lib.put("money", "is great");
    assert(c_lib.get("money") == "is great");
    std::this_thread::sleep_for (std::chrono::milliseconds(500));

}
