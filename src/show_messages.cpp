#include <memory> 
#include <future> 
#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "socket.cpp"
#include <cstring>
 
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"    
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"  
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"  
#include "ftxui/util/ref.hpp"
#include "ftxui/component/loop.hpp"

struct irc_msg {
  std::string prefix = "";
  std::string command;
  std::vector<std::string> params;
};

struct irc_msg parse_msg(std::string s) {
  std::string trailing = "";
  irc_msg msg;

  if (s.size() < 1) {
    std::cout << "there be no string";
  }
  if (s[0] == ':') {
    msg.prefix = s.substr(1, s.find(" "));
    s.erase(0, s.find(" ")+1);
  }
  msg.command = s.substr(0, s.find(" "));
  s.erase(0, s.find(" ")+1);
  if (s.find(" :") != std::string::npos) {
    trailing = s.substr(s.find(" :")+2, std::string::npos);
    s.erase(s.find(" :"), std::string::npos);
  }
  while (s.find(" ") != std::string::npos) {
    msg.params.push_back(s.substr(0,s.find(" ")));
    s.erase(0, s.find(" ")+1);
  }
  if (s.size()>0) {
    msg.params.push_back(s);
  }
  msg.params.push_back(trailing);

  return msg;
}

std::vector<irc_msg> recv_msgs(int sock_fd) {
  //std::cout << "\n new packet: \n";
  char buf[2048];
  int bytes_received;
  bytes_received = recv(sock_fd, buf, sizeof buf, 0);
  std::string str_buf = buf;
  std::vector<std::string> messages;
  std::vector<irc_msg> parsed_msgs;

  while (str_buf.find("\n") != std::string::npos) {
    messages.push_back(str_buf.substr(0, str_buf.find("\n")));
    str_buf.erase(0, str_buf.find("\n")+1);
  }

  for (std::string raw_msg : messages) {
    irc_msg msg = parse_msg(raw_msg);
    parsed_msgs.push_back(msg);
  }
  return parsed_msgs;
}

int login_irc(struct login_info *details, int sock_fd) {
  std::string nick_string = "NICK "+details->nick+"\n";
  std::string user_string = "USER guest 8 * :"+details->real_name+"\n";

  const char *nick = nick_string.c_str();
  const char *user = user_string.c_str(); 

  int len, bytes_sent;
  len = strlen(nick);
  bytes_sent = send(sock_fd, nick, len, 0);
  len = strlen(user);
  bytes_sent = send(sock_fd, user, len, 0);
  return 1;
  //std::string to_send = "PRIVMSG Guest98 :wahoo\n";
  //const char *do_send=  to_send.c_str(); 
  //len = strlen(do_send);
  //bytes_sent = send(sock_fdesc, do_send, len, 0);
}

int connect_irc() {
  u_long ip_bytes = htonl(inet_addr("94.125.182.252"));
  Catsock::CSocket server_socket = Catsock::CSocket(AF_INET,SOCK_STREAM,0,6667,ip_bytes);

  int sock_fdesc = server_socket.get_sock_fdesc();
  return sock_fdesc;
}

int main() {
	login_info connect_with;
	connect_with.nick = "cheese1";
	connect_with.real_name = "a";
	connect_with.password = "";
	int sock_fd = connect_irc();
	int login_success = login_irc(&connect_with, sock_fd);
}
