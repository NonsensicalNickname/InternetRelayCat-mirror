#include "socket.cpp"
#include <cstring>

int main() {
  u_long ip_bytes = htonl(inet_addr("127.0.0.1"));
  Catsock::BSocket server_socket = Catsock::BSocket(AF_INET,SOCK_STREAM,0,8080,ip_bytes);
  int socket_file_descriptor = server_socket.get_sock_fdesc();
  int socket_connection = server_socket.get_connection();

  listen(socket_file_descriptor, 5);
  std::cout << "server listening on port 8080\n";

  int new_fd;
  struct sockaddr_storage client_addr;
  socklen_t client_addr_size;

  client_addr_size = sizeof client_addr;
  new_fd = accept(socket_file_descriptor, (struct sockaddr *)&client_addr, &client_addr_size);

  constexpr const char *response_msg = "get responded idiot\n";
  int len, bytes_sent;
  len = strlen(response_msg);
  bytes_sent = send(new_fd, response_msg, len, 0);
  std::cout << "sent " << bytes_sent << " bytes to client\n";
}
