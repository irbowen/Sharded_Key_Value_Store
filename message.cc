
#include <sstream>
#include "message.h"

std::string Message::serialize() {
    char div_char = ':';
    std::ostringstream oss;

    oss << msg_type;
    oss << div_char;

    oss << view_num;
    oss << div_char;

    oss << seq_num;
    oss << div_char;

    oss << value;
    oss << div_char;

    oss << sender.port;
    oss << div_char;
    oss << sender.host;
    oss << div_char;

    oss << receivers.size();
    oss << div_char;
    for (auto& r : receivers) {
        oss << r.port;
        oss << div_char;
        oss << r.host;
        oss << div_char;
    }
    return oss.str();
}

void Message::deserialize(std::string in) {

    std::vector<std::string> array;
    size_t pos = 0, found;
    while ((found = in.find_first_of(':', pos)) != std::string::npos) {
        array.push_back(in.substr(pos, found - pos));
        pos = found + 1;
    }
    array.push_back(in.substr(pos));

    int index = 0;
    int msg_int = stoi(array.at(index++));
    msg_type = static_cast<MessageType>(msg_int);
    view_num = stoi(array.at(index++));
    seq_num = stoi(array.at(index++));
    value = array.at(index++);
    sender.port = stoi(array.at(index++));
    sender.host = array.at(index++);
    int num_recv = stoi(array.at(index++));
    int start = index;
    // cout << "Num recv: " << num_recv << endl;
    for (int k = 0; k < num_recv; k++) {
        int i = 2 * k;
        // cout << "at: " << array.at(start + i) << " " << array.at(start + 1 + i) << endl;
        node r;
        r.port = stoi(array.at(i + start));
        r.host = array.at(i + 1 + start);
        receivers.push_back(r);
    }
    // cout << "Num recv: " << num_recv << endl;
}

