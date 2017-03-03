## Isaac Bowen, eecs485win16p6

CXXFLAGS = -std=c++1z -g -pedantic -Wall -pthread

SERVER_LIB := server.o server_main.o
WORKER_LIB := worker.o worker_main.o

########################################
default: all 
all: server worker

DEPS := $(wildcard *.h)

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

server: $(SERVER_LIB)
	$(CXX) $(CXXFLAGS) -o $@ $^

worker: $(WORKER_LIB)
	$(CXX) $(CXXFLAGS) -o $@ $^

########################################
splitter: split.cc helper.h
	$(CXX) $(CXXFLAGS) split.cc -o splitter

########################################
clean:
	touch indexer server worker
	rm indexer server worker
	rm *.o

