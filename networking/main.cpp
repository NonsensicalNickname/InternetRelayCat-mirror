#include "bsocket.cpp"
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>

int main() {
  u_long ip_bytes = htonl(inet_addr("127.0.0.1"));
  Catsock::BSocket server_socket = Catsock::BSocket(AF_INET,SOCK_STREAM,0,8080,ip_bytes);
  int socket_file_descriptor = server_socket.get_sock_fdesc();
  int socket_connection = server_socket.get_connection();
  std::cout << socket_file_descriptor << " " << socket_connection;
}
