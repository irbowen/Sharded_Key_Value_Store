## Isaac Bowen, eecs485win16p6

CXXFLAGS = -std=c++1z -g -pedantic -Wall -pthread

BOTH_LIB := network.o message.o
SERVER_LIB := paxos_main.o replica.o acceptor.o learner.o proposer.o $(BOTH_LIB)
CLIENT_LIB := client_lib.o client1.o $(BOTH_LIB)

########################################
default: all 
all: paxos_server client1

DEPS := $(wildcard *.h)

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

client1: $(CLIENT_LIB)
	$(CXX) $(CXXFLAGS) -o $@ $^

paxos_server: $(SERVER_LIB)
	$(CXX) $(CXXFLAGS) -o $@ $^

########################################
clean:
	touch paxos_server client1
	rm paxos_server client1
	rm *.o

