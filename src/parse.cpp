#include <iostream>
#include <vector>

struct irc_message {
  std::string prefix;
  std::string command;
  std::vector<std::string> parameters;
};

std::string trim_trailing(std::string s) {
  return s.substr(s.find_first_not_of(" \n"), s.find_last_not_of(" \n")-s.find_first_not_of(" \n")+1);
}

std::vector<std::string> split(std::string s, std::string separator) {
  std::vector<int> spaces;
  std::vector<std::string> slices;
  bool done = false;
  int i;

  while (done==false) {
    if (spaces.size() > 0) {
      i = s.find(separator, spaces.back()+1);
    }
    else {
      i = s.find(separator);
    }
    if (i == std::string::npos) {
      done = true;
    }
    else {
      spaces.push_back(i);
    }
  }
  if (spaces.size() > 0) {
    for (int e=0; e<spaces.size(); e++) {
      if (e==0) {
        slices.push_back(s.substr(0, spaces.at(e)));
      }
      else {
        slices.push_back(s.substr(spaces.at(e-1)+1, spaces.at(e)-spaces.at(e-1)-1));
      }
    }
    slices.push_back(s.substr(spaces.back()+1));
  }
  else {
    slices.push_back(s);
  }
  return slices;
}

void parse_message(std::string message) {
  std::vector<std::string> slices = split(trim_trailing(message), " ");
  std::vector<std::string> params;
  std::string param_trailing;
  std::string prefix;
  std::string command;
  bool trailing = false;
  
  if (message.size() < 1) {
    std::cout << "nuh uh";
  }
  if (message[0] == ':') {
    prefix = slices.at(0).substr(1,slices.at(0).size()-1);
    slices.erase(slices.begin());
  }
  command = slices.at(0);
  slices.erase(slices.begin());

  for (std::string param : slices) {
    if (param[0] == ':') {
      trailing = true;
    }
    else if (trailing == true) {
      param_trailing.append(param+" "); 
    }
    else {
      params.push_back(param);
    }
  }
  params.push_back(trim_trailing(param_trailing));

  std::cout << "prefix: " << prefix << "\ncommand: " << command << "\nparameters: "; 
  for (std::string param : params) {
    std::cout << param << ",";
  }
}

int main() {
  char raw_msg[2048] = ":lead.libera.chat NOTICE * :*** No Ident response: n175-32-209-194.mas22.nsw.optusnet.com.au";
  std::string message = raw_msg;
  parse_message(message);
  
  return 0;
}
