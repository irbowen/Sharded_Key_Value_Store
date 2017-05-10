
#ifndef learner_h
#define learner_h


#include <fstream>
#include <map>
#include <mutex>
#include <string>

#include "message.h"
#include "environment.h"

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
    std::map<int, Score> score_map;
    Environment* env_;
public:
    /* KV Store needs access to the data log */
    std::vector<std::string> log;
    std::vector<Object> object_log;
    void init(Environment* env);

    // For all of the below methods, the caller is responsible for freeing the memory
    // used by the messages that are returned

    // Process the ACCEPT_VALUE msg, and create the correct response msg
    Message* handle_learn_msg(int view_num, int seq_num, std::string value, Message* msg);
    Message* broadcast_learn(int seq_num);

    std::string get_latest_value(std::string key);
    std::vector<std::string> get_all_keys();
    void print_log();
    int get_seqnum();
    int get_seqnum_with_skip();
};

#endif /* learner_h */
