#ifndef Handler_hpp
#define Handler_hpp

#include "socket.cpp"
#include "config.cpp"

#include <string>
#include <chrono>
#include <thread>
#include <fcntl.h>
#include <poll.h>

namespace IRCat {
    class handler {
        private: 
   //         struct sockaddr_in address;
			struct irc_msg {
				std::string prefix = "";
				std::string command;
				std::vector<std::string> params;
			};
            int sock_fd;
            int connection;
			struct irc_msg parse_msg(std::string s);
			std::vector<irc_msg> recv_msgs();//int sock_fd) {
			int send_message(std::string message_body);
			ftxui::Element construct_msg(std::string origin, std::string body);
			IRCat::Config config_opts;
			std::vector<irc_msg> msg_data;
			struct pollfd pfds[1];
        public:
			ftxui::Element render_messages();
			struct login_info {
				std::string nick;
				std::string real_name;
				std::string password;
			};
            handler(login_info* details);
			bool poll_msgs();
			void send_user_msg(std::string contents);
    };
}

#endif
