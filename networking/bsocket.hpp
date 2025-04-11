#ifndef BSocket_hpp
#define BSocket_hpp

#include "socket.cpp"

namespace Catsock {
  class BSocket : public Socket {
    public:
      BSocket(int family, int service, int protocol, int port, u_long interface);
      int establish_connection(int sock_fdesc, struct sockaddr_in address);
  };
}

#endif
