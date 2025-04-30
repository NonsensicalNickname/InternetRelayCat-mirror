#include <memory> 
#include <future> 
#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

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

struct login_info {
  std::string nick;
  std::string real_name;
  std::string password;
};

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

struct message_visible {
  std::string origin;
  std::string body;
};

std::string send_message() {
  using namespace ftxui;
 
  auto screen = ScreenInteractive::TerminalOutput();
  std::string message_contents;
  std::string button_send_label = "Send";
  int mode = 0;
 
  Component input_message = Input(&message_contents, "send message");
  Component button_send = Button(&button_send_label, screen.ExitLoopClosure(), ButtonOption::Ascii());
 
  auto component = Container::Horizontal ({
    input_message,
    button_send,
  });
 
  auto renderer = Renderer(component, [&] {
    return vbox ({
      hbox(input_message->Render(),separator(), button_send->Render()),
    }) | border;
  });

  renderer |= CatchEvent([&](Event event) {
    if (event == Event::Character('\e')) {
      screen.ExitLoopClosure()();
      return true;
    }
    else if (event == Event::Character('\n')) {
      screen.ExitLoopClosure()();
      return true;
    }
    return false;
  });
 
  screen.Loop(renderer);
  return(message_contents);
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

struct login_info login() {
  using namespace ftxui;
 
  auto screen = ScreenInteractive::TerminalOutput();

  std::string nick;
  std::string real_name;
  std::string password;
  std::string button_submit_label = "Submit";
 
  Component input_nick = Input(&nick, "nick");
  Component input_real_name = Input(&real_name, "real name");
  Component button_submit = Button(&button_submit_label, screen.ExitLoopClosure());
 
  InputOption password_option;
  password_option.password = true;
  Component input_password = Input(&password, "password", password_option);
 
  auto component = Container::Vertical({
      input_nick,
      input_real_name,
      input_password,
      button_submit,
  });
 
  auto renderer = Renderer(component, [&] {
    return vbox({
      hbox(text(" User nick  : "), input_nick->Render()),
      hbox(text(" Real name  : "), input_real_name->Render()),
      hbox(text(" Password   : "), input_password->Render()),
      separator(),
      button_submit->Render()
    }) | border;
  });
 
  screen.Loop(renderer);
  login_info submitted_details;
  submitted_details.nick = nick;
  submitted_details.real_name = real_name;
  submitted_details.password = password;
  return(submitted_details);
}

int connect_irc(struct login_info *details) {
  u_long ip_bytes = htonl(inet_addr("94.125.182.252"));
  Catsock::CSocket server_socket = Catsock::CSocket(AF_INET,SOCK_STREAM,0,6667,ip_bytes);

  int sock_fd = server_socket.get_sock_fd();
  fcntl(sock_fd, F_SETFL, O_NONBLOCK);

  std::string nick_string = "NICK "+details->nick+"\n";
  std::string user_string = "USER guest 8 * :"+details->real_name+"\n";

  const char *nick = nick_string.c_str();
  const char *user = user_string.c_str(); 

  int len, bytes_sent;
  len = strlen(nick);
  bytes_sent = send(sock_fd, nick, len, 0);
  len = strlen(user);
  bytes_sent = send(sock_fd, user, len, 0);
  return sock_fd;
}

ftxui::Element construct_msg(std::string origin, std::string body) {
  using namespace ftxui;

  return vbox (
	  paragraph(origin),
	  paragraph(body),
	  separator()
  ) | flex;
};


ftxui::Element render_messages(std::vector<irc_msg> msg_data) {
  using namespace ftxui;

  std::vector<Elements> constructed_msgs;

  for (irc_msg msg : msg_data) {
    constructed_msgs.push_back({construct_msg(msg.prefix, msg.params.back())});
  }
 
  return gridbox(constructed_msgs);
}

void main_ui(int sock_fd) {
  using namespace ftxui;

  auto screen = ScreenInteractive::Fullscreen();
  std::string message_contents;
  std::string button_send_label = "Send";
  std::vector<irc_msg> msg_data;
  float scroll_percent = 0.0f;

  std::vector<std::string> options = {"one", "two", "three"};
  int selectedOpt = 0;
  MenuOption option;
  option.on_enter = screen.ExitLoopClosure();
 
  Component menu = Menu(&options, &selectedOpt, option);
  Component input_message = Input(&message_contents, "send message");
  Component button_send = Button(&button_send_label, screen.ExitLoopClosure(), ButtonOption::Ascii());

  auto message_box = Container::Horizontal ({
    input_message,
    button_send,
  });

  auto components = Container::Vertical ({
    message_box,
    menu,
  });
 
  auto renderer = Renderer(components, [&] {
    return vbox ({
  //    menu->Render(),
      filler(),
      render_messages(msg_data) | focusPositionRelative(0, scroll_percent) | frame | vscroll_indicator 
		| size(HEIGHT, LESS_THAN, Terminal::Size().dimy * 0.8),
      hbox(input_message->Render() | selectionBackgroundColor(Color::Blue), separator(), button_send->Render()) | borderLight | size(HEIGHT, LESS_THAN, Terminal::Size().dimy * 0.1),
    }) | borderHeavy;
  });

  renderer |= CatchEvent([&](Event event) {
    if (event == Event::Character('\n')) {
      screen.ExitLoopClosure()();
      return true;
    }
    if (event.mouse().button == Mouse::WheelUp && scroll_percent>=0.01) {
	  scroll_percent-=0.01;
      return true;
    }
    if (event.mouse().button == Mouse::WheelDown && scroll_percent<=0.99) {
	  scroll_percent+=0.01;
      return true;
    }
    return false;
  });
 
  Loop loop(&screen, renderer);
 
  struct pollfd pfds[1];
  pfds[0].fd = sock_fd;
  pfds[0].events = POLLIN;

  while (!loop.HasQuitted()) {
	int num_events = poll(pfds, 1, 10);
	if (num_events > 0) {
	  std::vector<irc_msg> new_msgs = recv_msgs(sock_fd);
	  msg_data.insert(msg_data.end(), new_msgs.begin(), new_msgs.end());
	}

    loop.RunOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

int main() {
  login_info connect_with;
  connect_with.nick = "cheese1";
  connect_with.real_name = "a";
  connect_with.password = "";

  int sock_fd = connect_irc(&connect_with);
  main_ui(sock_fd);
}
