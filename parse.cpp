#include <iostream>
#include <vector>

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

int main() {
  std::string s = ":lead.libera.chat 265 cheese452 1990 2907 :Current local users 1990, max 2907";
  irc_msg msg = parse_msg(s);

  std::cout << "prefix=" << msg.prefix << "\ncommand=" << msg.command << "\n";
  for (std::string ss : msg.params) {
    std::cout << ss << ",";
  }
}
