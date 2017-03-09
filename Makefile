
DEBUG = -g
RELEASE = -O3
VERSION = $(DEBUG)

CXXFLAGS = -std=c++1z $(VERSION) -pedantic -Wall -pthread

BOTH_LIB := network.o message.o
SERVER_LIB := paxos_main.o replica.o acceptor.o learner.o proposer.o $(BOTH_LIB)
CLIENT_LIB := client_lib.o $(BOTH_LIB)

########################################
default: all 
all: paxos_server clients

DEPS := $(wildcard *.h)

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

paxos_server: $(SERVER_LIB)
	$(CXX) $(CXXFLAGS) -o $@ $^

CLIENT_SRC :=$(wildcard chat_client_*.cc)
CLIENTS := $(patsubst %.cc,%.out,$(CLIENT_SRC))

clients: $(CLIENTS)

chat_client_%.out: chat_client_%.o $(CLIENT_LIB)
	$(CXX) $(CXXFLAGS) -o $@ $^

########################################
clean:
	touch paxos_server client1
	rm paxos_server client1
	touch dummy.o dummy.out log_0.txt
	rm *.o *.out log_*.txt
