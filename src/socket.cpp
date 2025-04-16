#include "socket.hpp"

Catsock::Socket::Socket(int family, int service, int protocol, int port, u_long interface) {

  address.sin_family = family;
  address.sin_port = htons(port);
  address.sin_addr.s_addr = htonl(interface);

  sock_fdesc = socket(family, service, protocol);
  check_connection(sock_fdesc);
}

void Catsock::Socket::check_connection(int val) {
  if (val < 0) {
    perror("a fucky wucky occured while establishing the connection");
    exit(EXIT_FAILURE);
  }
}

struct sockaddr_in Catsock::Socket::get_address() {
  return address;
}
int Catsock::Socket::get_sock_fdesc() {
  return sock_fdesc;
}
int Catsock::Socket::get_connection() {
 return connection;
}
void Catsock::Socket::set_connection(int target) {
  connection = target;
}
