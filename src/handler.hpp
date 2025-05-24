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
			struct irc_msg {
				std::string prefix = "";
				std::string command;
				std::vector<std::string> params;
			};

            int sock_fd;
			struct pollfd pfds[1];

			struct irc_msg parse_msg(std::string s);
			std::vector<irc_msg> recv_msgs();//int sock_fd) {
			ftxui::Element construct_msg(std::string origin, std::string body);
			int send_message(std::string message_body);

			IRCat::Config config_opts;
			std::vector<irc_msg> msg_data;
        public:
			ftxui::Element render_messages();
			struct login_info {
				std::string nick;
				std::string real_name;
				std::string password;
			};
			struct IRCat::User user;
			struct IRCat::Server server;
            handler();//login_info* details);
			bool poll_msgs();
			int send_user_msg(std::string contents, std::string channel);
			void set_channel(std::string target);
    };
}

#endif
