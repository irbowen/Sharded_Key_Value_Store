
DEBUG = -g
RELEASE = -O3
VERSION = $(RELEASE)

CXXFLAGS = -std=c++1z $(VERSION) -pedantic

BOTH_LIB := objs/network.o objs/message.o
SERVER_LIB := objs/paxos_main.o objs/replica.o objs/acceptor.o objs/learner.o objs/proposer.o $(BOTH_LIB)
CLIENT_LIB := objs/client_lib.o $(BOTH_LIB)

########################################
default: all
all: paxos_server clients

DEPS := $(wildcard *.h)

.PRECIOUS: objs/%.o
objs/%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

paxos_server: $(SERVER_LIB)
	$(CXX) $(CXXFLAGS) -o bin/$@ $^

CLIENT_SRC :=$(wildcard chat_client_*.cc)
CLIENTS := $(patsubst %.cc,%.out,$(CLIENT_SRC))

clients: $(CLIENTS)

chat_client_%.out: objs/chat_client_%.o $(CLIENT_LIB)
	$(CXX) $(CXXFLAGS) -o bin/$@ $^

########################################
clean:
	touch bin/paxos_server
	touch log_0.txt
	touch objs/dummy.o bin/dummy.out
	rm objs/*.o bin/*.out log_*.txt bin/paxos_server
