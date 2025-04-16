#include <memory>  // for allocator, __shared_ptr_access
#include <string>  // for char_traits, operator+, string, basic_string
#include <iostream>

#include "csocket.cpp"
#include <cstring>
 
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/util/ref.hpp"  // for Ref

struct login_info {
  std::string nick;
  std::string real_name;
  std::string password;
};

struct login_info login() {
  using namespace ftxui;
 
  auto screen = ScreenInteractive::TerminalOutput();
  // The data:
  std::string nick;
  std::string real_name;
  std::string password;
  std::string button_submit_label = "Submit";
 
  // The basic input components:
  Component input_nick = Input(&nick, "nick");
  Component input_real_name = Input(&real_name, "real name");
  Component button_submit = Button(&button_submit_label, screen.ExitLoopClosure());
 
  // The password input component:
  InputOption password_option;
  password_option.password = true;
  Component input_password = Input(&password, "password", password_option);
 
  // The component tree:
  auto component = Container::Vertical({
      input_nick,
      input_real_name,
      input_password,
      button_submit,
  });
 
  // Tweak how the component tree is rendered:
  auto renderer = Renderer(component, [&] {
    return vbox({
      hbox(text(" User nick  : "), input_nick->Render()),
      hbox(text(" Real name  : "), input_real_name->Render()),
      hbox(text(" Password   : "), input_password->Render()),
      separator(),
      //hbox(
      //  vbox(
      //    text("Hello " + nick + real_name),
      //    text("Your password is " + password)
      //  ),
        button_submit->Render()
      //),
    }) | border;
  });
 
  screen.Loop(renderer);
  login_info submitted_details;
  submitted_details.nick = nick;
  submitted_details.real_name = real_name;
  submitted_details.password = password;
  return(submitted_details);
}

void connect_irc(struct login_info *details) {
  std::string nick_string = "NICK "+details->nick+"\n";
  std::string user_string = "USER guest 8 * :"+details->real_name+"\n";
  const char *nick = nick_string.c_str();
  const char *user = user_string.c_str(); 

  u_long ip_bytes = htonl(inet_addr("94.125.182.252"));
  Catsock::CSocket server_socket = Catsock::CSocket(AF_INET,SOCK_STREAM,0,6667,ip_bytes);

  int sock_fdesc = server_socket.get_sock_fdesc();

  int len, bytes_sent;
  len = strlen(nick);
  bytes_sent = send(sock_fdesc, nick, len, 0);
  len = strlen(user);
  bytes_sent = send(sock_fdesc, user, len, 0);

  while (1) {
    std::cout << "\n new packet: \n";
    char buf[2048];
    int bytes_received;

    bytes_received = recv(sock_fdesc, buf, sizeof buf, 0);
    std::cout << buf << "\n";
  }
}

int main() {
  login_info connect_with = login();
  connect_irc(&connect_with);
}
