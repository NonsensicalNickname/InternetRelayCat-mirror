#include "csocket.cpp"
#include <cstring>

int main() {
  u_long ip_bytes = htonl(inet_addr("127.0.0.1"));
  Catsock::CSocket server_socket = Catsock::CSocket(AF_INET,SOCK_STREAM,0,8080,ip_bytes);
  int socket_file_descriptor = server_socket.get_sock_fdesc();
  int socket_connection = server_socket.get_connection();
  std::cout << socket_file_descriptor << " " << socket_connection << "\n";

  char buf[32];
  int bytes_received;

  bytes_received = recv(socket_file_descriptor, buf, sizeof buf, 0);
  std::cout << buf;
}
