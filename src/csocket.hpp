#ifndef CSocket_hpp
#define CSocket_hpp

#include "socket.cpp"

namespace Catsock {
  class CSocket : public Socket {
    public:
      CSocket(int family, int service, int protocol, int port, u_long interface);
      int establish_connection(int sock_fdesc, struct sockaddr_in address);
  };
}

#endif
