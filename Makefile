
DEBUG = -g
RELEASE = -O3
VERSION = $(RELEASE)

CXXFLAGS = -std=c++14 $(VERSION) -pedantic -pthread

BOTH_LIB := objs/network.o objs/message.o
REPLICA_LIB := objs/paxos_main.o  objs/replica.o objs/acceptor.o objs/learner.o objs/proposer.o objs/kv_store.o $(BOTH_LIB) objs/paxos.o
CLIENT_LIB := objs/client_lib.o $(BOTH_LIB)
MASTER_LIB := objs/master_main.o objs/master.o objs/shard.o $(BOTH_LIB)

########################################
default: all
all: paxos_server kv_master clients

DEPS := $(wildcard headers/*.h)

.PRECIOUS: objs/%.o
objs/%.o: src/%.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

paxos_server: $(REPLICA_LIB)
	$(CXX) $(CXXFLAGS) -o bin/$@ $^

kv_master: $(MASTER_LIB)
	$(CXX) $(CXXFLAGS) -o bin/$@ $^

CLIENT_SRC :=$(wildcard src/kv_client_*.cc)
CLIENTS := $(patsubst src/%.cc,%.out,$(CLIENT_SRC))

clients: $(CLIENTS)

kv_client_%.out: objs/kv_client_%.o $(CLIENT_LIB)
	$(CXX) $(CXXFLAGS) -o bin/$@ $^


########################################
clean:
	touch bin/paxos_server bin/kv_master
	touch log_0.txt
	touch objs/dummy.o bin/dummy.out
	rm objs/*.o bin/*.out log_*.txt bin/kv_master bin/paxos_server
