#include "csocket.cpp"
#include <cstring>

int main() {
  u_long ip_bytes = htonl(inet_addr("94.125.182.252"));
  Catsock::CSocket server_socket = Catsock::CSocket(AF_INET,SOCK_STREAM,0,6667,ip_bytes);
  int socket_file_descriptor = server_socket.get_sock_fdesc();
  int socket_connection = server_socket.get_connection();
  std::cout << socket_file_descriptor << " " << socket_connection << "\n";

  constexpr const char *nick = "NICK cheese432\n";
  constexpr const char *user = "USER guest 8 * :cheese432\n";
  int len, bytes_sent;
  len = strlen(nick);
  bytes_sent = send(socket_file_descriptor, nick, len, 0);
  len = strlen(user);
  bytes_sent = send(socket_file_descriptor, user, len, 0);

  while (1) {
    char buf[1024];
    int bytes_received;

    bytes_received = recv(socket_file_descriptor, buf, sizeof buf, 0);
    std::cout << buf;
  }
}
