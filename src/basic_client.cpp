#include "csocket.cpp"
#include <cstring>

int main() {
  u_long ip_bytes = htonl(inet_addr("127.0.0.1"));
  Catsock::CSocket server_socket = Catsock::CSocket(AF_INET,SOCK_STREAM,0,8080,ip_bytes);
  int socket_file_descriptor = server_socket.get_sock_fdesc();
  int socket_connection = server_socket.get_connection();

  std::cout << "connected to server on port 8080\n";

  char buf[512];
  int bytes_received;
  
  std::cout << "received message from server:\n";
  bytes_received = recv(socket_file_descriptor, buf, sizeof buf, 0);
  std::cout << buf;
}
