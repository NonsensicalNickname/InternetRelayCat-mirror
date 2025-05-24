
#include "handler.hpp"
#include <string>

IRCat::Handler::Handler() {
	config_opts = IRCat::Config();
	user = config_opts.users[config_opts.default_user];
	server = config_opts.server;

    u_long ip_bytes = htonl(inet_addr(server.ip_addr.c_str()));
    Catsock::CSocket server_socket = Catsock::CSocket(AF_INET,SOCK_STREAM,0,6667,ip_bytes);

    sock_fd = server_socket.get_sock_fd();

    fcntl(sock_fd, F_SETFL, O_NONBLOCK);
    pfds[0].fd = sock_fd;
    pfds[0].events = POLLIN;

    std::string nick_string = "NICK "+user.nick;
    std::string user_string = "USER guest "+std::to_string(user.status)+" * :"+user.real_name;

    int bytes_sent;

    bytes_sent = send_message(nick_string);
    bytes_sent = send_message(user_string);
}

int IRCat::Handler::send_message(std::string message_body) {
	const char *msg = message_body.append("\n").c_str();
	int len, bytes_sent;
	int total_sent = 0;
	len = strlen(msg);

	while (total_sent < len) {
		bytes_sent = send(sock_fd, msg, len, 0);
		total_sent+= bytes_sent;
	}
	return total_sent;
}

struct IRCat::Handler::irc_msg IRCat::Handler::parse_msg(std::string s) {
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

std::vector<IRCat::Handler::irc_msg> IRCat::Handler::recv_msgs() {
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

ftxui::Element IRCat::Handler::construct_msg(std::string origin, std::string body) {
    using namespace ftxui;

    return vbox (
	    separator() | color(config_opts.theme[IRCat::fg]),
	    paragraph(origin) | color(config_opts.theme[IRCat::msg_origin]),
	    paragraph(body) | color(config_opts.theme[IRCat::msg_body])
    ) | flex;
};


ftxui::Element IRCat::Handler::render_messages() {
    using namespace ftxui;

    std::vector<Elements> constructed_msgs;

    for (irc_msg msg : msg_data) {
		if (msg.command != "PONG") {
			constructed_msgs.push_back({construct_msg(msg.prefix, msg.params.back())});
		}
		else {
			send_message(":PONG "+msg.params[0]);
		}
    }
 
    return gridbox(constructed_msgs);
}

int IRCat::Handler::send_user_msg(std::string contents, std::string channel) {
	std::string pr = ":"+user.nick+" PRIVMSG " + channel + " :"+contents;
	int bytes_sent = send_message(pr);
	msg_data.push_back(parse_msg(pr));
	return bytes_sent;
}

bool IRCat::Handler::poll_msgs() {
	int num_events = poll(pfds, 1, 10);
	if (num_events > 0) {
		std::vector<irc_msg> new_msgs = recv_msgs();
		msg_data.insert(msg_data.end(), new_msgs.begin(), new_msgs.end());
		return true;
	}
	else { 
		return false;
	}
}
