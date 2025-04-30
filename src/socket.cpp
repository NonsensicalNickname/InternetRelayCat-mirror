#include "socket.hpp"

Catsock::Socket::Socket(int family, int service, int protocol, int port, u_long interface) {

  address.sin_family = family;
  address.sin_port = htons(port);
  address.sin_addr.s_addr = htonl(interface);

  sock_fd = socket(family, service, protocol);
  check_connection(sock_fd);
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
int Catsock::Socket::get_sock_fd() {
  return sock_fd;
}
int Catsock::Socket::get_connection() {
 return connection;
}
void Catsock::Socket::set_connection(int target) {
  connection = target;
}

Catsock::BSocket::BSocket(int family, int service, int protocol, int port, u_long interface) 
  : Socket(family, service, protocol, port, interface) {

  int opt_on = 1;
  setsockopt(get_sock_fd(), SOL_SOCKET, SO_REUSEADDR, &opt_on, sizeof opt_on);
  set_connection(establish_connection(get_sock_fd(), get_address()));
  check_connection(get_connection());
}

int Catsock::BSocket::establish_connection(int sock_fd, struct sockaddr_in address) {
  return bind(sock_fd, (struct sockaddr *)&address, sizeof address);
}

Catsock::CSocket::CSocket(int family, int service, int protocol, int port, u_long interface) 
  : Socket(family, service, protocol, port, interface) {

  set_connection(establish_connection(get_sock_fd(), get_address()));
  check_connection(get_connection());
}

int Catsock::CSocket::establish_connection(int sock_fd, struct sockaddr_in address) {
  return connect(sock_fd, (struct sockaddr *)&address, sizeof address);
}
