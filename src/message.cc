
#include <sstream>
#include "../headers/message.h"

std::string Message::serialize() {
    std::ostringstream oss;

    oss << msg_type;
    oss << div_char;

    oss << view_num;
    oss << div_char;

    oss << seq_num;
    oss << div_char;

    oss << key;
    oss << div_char;

    oss << value;
    oss << div_char;

    oss << sender.port_;
    oss << div_char;
    oss << sender.host_;
    oss << div_char;

    oss << receivers.size();
    oss << div_char;
    for (auto& r : receivers) {
        oss << r.port_;
        oss << div_char;
        oss << r.host_;
        oss << div_char;
    }

    oss << acceptor_state.size();
    oss << div_char;
    for (auto& v : acceptor_state) {
        oss << v.view_num;
        oss << div_char;
        oss << v.value;
        oss << div_char;
    }

    oss << all_keys.size();
    oss << div_char;
    for (auto &v : all_keys){
        oss << v;
        oss << div_char;
    }

    return oss.str();
}

void Message::deserialize(std::string in) {
    std::vector<std::string> array;
    size_t pos = 0, found;
    while ((found = in.find_first_of(div_char, pos)) != std::string::npos) {
        array.push_back(in.substr(pos, found - pos));
        pos = found + 1;
    }
    array.push_back(in.substr(pos));

    int index = 0;

    int msg_int = stoi(array.at(index++));
    msg_type = static_cast<MessageType>(msg_int);

    view_num = stoi(array.at(index++));
    seq_num = stoi(array.at(index++));
    key = array.at(index++);
    value = array.at(index++);
    sender.port_ = stoi(array.at(index++));
    sender.host_ = array.at(index++);
    int num_recv = stoi(array.at(index++));
    // COUT << "Num recv: " << num_recv << endl;
    for (int k = 0; k < num_recv; k++) {
        // COUT << "at: " << array.at(start + i) << " " << array.at(start + 1 + i) << endl;
        node r;
        r.port_ = stoi(array.at(index++));
        r.host_ = array.at(index++);
        receivers.push_back(r);
    }
    num_recv = stoi(array.at(index++));
    for (int k = 0; k < num_recv; k++) {
        view_val v;
        v.view_num = stoi(array.at(index++));
        v.value = array.at(index++);
        acceptor_state.push_back(v);
    }
    int num_keys = stoi(array.at(index++));
    for(int k = 0;k < num_keys; k++){
        all_keys.push_back(array.at(index++));
    }
    // COUT << "Num recv: " << num_recv << endl;
}

bool Message::is_paxos_msg() {
    return (msg_type == MessageType::START_PREPARE ||
        msg_type == MessageType::PREPARE ||
        msg_type == MessageType::PREPARE_ACCEPT ||
        msg_type == MessageType::PREPARE_REJECT ||
        msg_type == MessageType::PROPOSE ||
        msg_type == MessageType::PROPOSE_ACCEPT ||
        msg_type == MessageType::PROPOSE_REJECT ||
        msg_type == MessageType::ACCEPT_VALUE ||
        msg_type == MessageType::PROPOSAL_LEARNT);
}

bool Message::is_kv_msg() {
    return (msg_type == MessageType::PUT ||
        msg_type == MessageType::GET ||
        msg_type == MessageType::DELETE ||
        msg_type == MessageType::ADD_SHARD ||
        msg_type == MessageType::MASTER_ACK ||
        msg_type == MessageType::GET_KEYS);
}

std::vector<std::string> split(std::string in) {
    std::vector<std::string> array;
    size_t pos = 0, found;
    while ((found = in.find_first_of('#', pos)) != std::string::npos) {
        array.push_back(in.substr(pos, found - pos));
        pos = found + 1;
    }
    array.push_back(in.substr(pos));
    return array;
}

std::string Message::get_key() {
    return split(value).at(0);
}

std::string Message::get_value() {
    return split(value).at(1);
}

std::string Message::get_client_id() {
    auto array = split(value);
    return array.at(2) + "#" + array.at(3);
}

int Message::get_client_seq_num() {
    return stoi(split(value).at(4));
}

node Message::get_client_node() {
    auto array =  split(value);
    return node(stoi(array.at(2)), array.at(3));
}
