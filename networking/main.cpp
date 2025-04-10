#include "socket.cpp"
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>

int main() {
  Catsock::Socket server_socket = Catsock::Socket(AF_INET,SOCK_STREAM,0,3497,2130706433);
  int socket_file_descriptor = server_socket.get_sock_fdesc();
  int socket_connection = server_socket.get_connection();
  std::cout << socket_file_descriptor << " " << socket_connection;
}
