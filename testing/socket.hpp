#ifndef Socket_hpp
#define Socket_hpp

#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>

const u_long LOCALHOST_IP = htonl(inet_addr("127.0.01"));
const int PORT = 8080;

namespace Catsock {
  class Socket {

    private: 
      struct sockaddr_in address;
//      struct addrinfo hints;
 //     struct addrinfo *address_res;
      int sock_fdesc;
      int connection;
    public:
      Socket(int family, int service, int protocol, int port=PORT, u_long interface=LOCALHOST_IP);
      void check_connection(int val);
      struct sockaddr_in get_address();
      int get_sock_fdesc();
      int get_connection();
  };
}

#endif
