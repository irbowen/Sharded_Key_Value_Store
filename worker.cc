
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <arpa/inet.h>

#include "worker.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#define MAXDATASIZE 100 // max number of bytes we can get at once 

using namespace std;

int32_t send_on_socket(int32_t socket, std::string msg) {
  int32_t return_value = 0;
  if ((return_value = send(socket, msg.c_str(), msg.length(), MSG_NOSIGNAL)) == -1) {
    std::cout << "Failed to send: (" << msg << ") to worker\n"; 
  }
  close(socket);
  return return_value;
}

void* get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void worker::process_request(int32_t socket) {
  send_on_socket(socket, "This worker has received your msg. This is your response.");
  // This should be a dispatcher based on msg type
  // Then, one of the dispatches should be to register a worker
}

worker::worker(std::string master_hostname, int master_port,
    std::string worker_hostname, int worker_port) :
  master_port_(master_port), master_hostname_(master_hostname),
  worker_port_(worker_port), worker_hostname_(worker_hostname) {
    // Silence is golden
  }

void worker::register_with_master() {
  int sockfd, numbytes;  
  char buf[MAXDATASIZE];
  struct addrinfo hints, *servinfo, *p;
  int rv;
  char s[INET6_ADDRSTRLEN];
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((rv = getaddrinfo(master_hostname_.c_str(), std::to_string(master_port_).c_str(), &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return;
  }
  // loop through all the results and connect to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
            p->ai_protocol)) == -1) {
      perror("client: socket");
      continue;
    }
    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("client: connect");
      continue;
    }
    break;
  }
  if (p == NULL) {
    fprintf(stderr, "client: failed to connect\n");
    return;
  }
  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
      s, sizeof s);
  printf("client: connecting to %s\n", s);
  freeaddrinfo(servinfo); // all done with this structure

  if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
    perror("recv");
    exit(1);
  }

  buf[numbytes] = '\0';
  printf("client: received '%s'\n",buf);
  if (send(sockfd, "Hello, world!", 13, 0) == -1)
    perror("send");
  close(sockfd);
}

void worker::setup_worker() {
  // Lock so no other threads can do anything until this is done
  unique_lock<mutex> lock(startup_mutex_);

  int status;
  struct addrinfo hints, *servinfo, *p;
  memset(&hints, 0, sizeof hints);
  // Do we care about ipv4 vs ipv6?
  hints.ai_family = AF_UNSPEC;
  // TCP
  hints.ai_socktype = SOCK_STREAM;
  // Fill in the ip address on my machine
  hints.ai_flags = AI_PASSIVE;
  if ((status = getaddrinfo("localhost", std::to_string(worker_port_).c_str(), &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }
  int sock_fd, new_fd;
  int yes = 1;
  // loop through all the results and bind to the first we can
  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      perror("setsockopt");
      exit(1);
    }
    if (bind(sock_fd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sock_fd);
      perror("server: bind");
      continue;
    }
    break;
  }
  freeaddrinfo(servinfo);
  if (p == NULL) {
    throw std::runtime_error("Failed to find a result to bind to");
  }
  if (listen(sock_fd, 10) == -1) {
    throw std::runtime_error("Failed to listen on a socket");
  }
  struct sockaddr_in their_addr;
  socklen_t sin_size;
  char s[INET6_ADDRSTRLEN];
  cout << "sock_fd " << sock_fd << endl;

  // We have done everything we need to do for setup.
  // We are now ready to start listening for jobs
  startup_ready_ = true;
  lock.unlock();
  startup_cv_.notify_all();

  register_with_master();

  while (true) {
    sin_size = sizeof their_addr;
    new_fd = accept(sock_fd, (struct sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1) {
      std::cerr << "Could not accept connection\n";
      continue;
    }
    inet_ntop(their_addr.sin_family, get_in_addr((struct sockaddr*)&their_addr), s, sizeof s);
    std::cout << "Server got connection from: " << s << std::endl;
    /* Create a new worker to hand this request */
    std::thread worker_thread([this, new_fd]() {
        process_request(new_fd);
        });
    worker_thread.detach();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}
