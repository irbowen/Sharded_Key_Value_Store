
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "helper.h"
#include "server.h"
#include "json.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

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

/* Much like the name says, prints the help menu
   and then exits the program */
void server::print_help_and_exit() {
  std::cout << "\nThis program takes a few command line arguments:\n"
    << "--port_number NUMBER, or -p NUMBER\n"
    << "\tIt will listen for requests on this port, and use a few ports above\n "
    << "\tit to communicate with clients/slaves\n"
    << "--help, or -h\n"
    << "\tDisplays the help menu\n"
    << "\n";
  exit(0);
}

void server::process_request(int32_t socket) {
  send_on_socket(socket, "The mapreduce master has received your msg. This is your response.");
}


void server::setup_server_for_requests(int port_number, std::string host) {
  int status;
  struct addrinfo hints, *servinfo, *p;
  memset(&hints, 0, sizeof hints);
  // Do we care about ipv4 vs ipv6?
  hints.ai_family = AF_UNSPEC;
  // TCP
  hints.ai_socktype = SOCK_STREAM;
  // Fill in the ip address on my machine
  hints.ai_flags = AI_PASSIVE;
  if ((status = getaddrinfo("localhost", std::to_string(port_number).c_str(), &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }
  int sock_fd, new_fd;
  int yes = 1;
  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
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

  // Start listening on a port
  while (1) {
    cout << "sup\n";
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



