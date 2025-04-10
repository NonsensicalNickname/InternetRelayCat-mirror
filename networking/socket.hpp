#ifndef Socket_hpp
#define Socket_hpp

#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>

namespace Catsock {
  class Socket {

    private: 
      struct sockaddr_in address;
      int sock_fdesc;
      int connection;
    public:
      Socket(int family, int service, int protocol, int port, u_long interface);
      void check_connection(int val);
      struct sockaddr_in get_address();
      int get_sock_fdesc();
      int get_connection();
  };
}

#endif
