#include "csocket.hpp"

Catsock::CSocket::CSocket(int family, int service, int protocol, int port, u_long interface) 
  : Socket(family, service, protocol, port, interface) {

  set_connection(establish_connection(get_sock_fdesc(), get_address()));
  check_connection(get_connection());
}

int Catsock::CSocket::establish_connection(int sock_fdesc, struct sockaddr_in address) {
  return connect(sock_fdesc, (struct sockaddr *)&address, sizeof address);
}
