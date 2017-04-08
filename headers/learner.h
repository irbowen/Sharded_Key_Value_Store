
#ifndef learner_h
#define learner_h


#include <fstream>
#include <map>
#include <mutex>
#include <experimental/optional>
#include <string>
#include <thread>
#include "message.h"

struct Score {
    size_t tally;
    std::string value;
};

struct Object {
    std::string key;
    std::string value;
    Object(std::string _key, std::string _value) : key(_key), value(_value) {}
    Object(){}
};

class Learner {
private:
    std::mutex m;
    size_t quorum;
    size_t id;
    std::map<int, Score> score_map;

public:
    /* KV Store needs access to the data log */
    std::vector<std::string> log;
    std::vector<Object> object_log;
    void init(size_t replica_count, size_t _id);

    // For all of the below methods, the caller is responsible for freeing the memory
    // used by the messages that are returned

    // Process the ACCEPT_VALUE msg, and create the correct response msg
    Message* handle_learn_msg(int view_num, int seq_num, std::string value, Message* msg);
    Message* broadcast_learn(int seq_num);

    std::experimental::optional<std::string> get_latest_value(std::string key);
    void print_log();
    int get_seqnum();
    int get_seqnum_with_skip();
};

#endif /* learner_h */
