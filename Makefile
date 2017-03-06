## Isaac Bowen, eecs485win16p6

CXXFLAGS = -std=c++1z -g -pedantic -Wall -pthread

SERVER_LIB := paxos_main.o replica.o acceptor.o learner.o proposer.o

########################################
default: all 
all: paxos_server

DEPS := $(wildcard *.h)

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

paxos_server: $(SERVER_LIB)
	$(CXX) $(CXXFLAGS) -o $@ $^

########################################
clean:
	touch paxos_server
	rm paxos_server
	rm *.o

