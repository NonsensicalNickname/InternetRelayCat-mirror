#ifndef Config_hpp
#define Config_hpp

#include <fstream>
#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <filesystem>
#include <unistd.h>
#include <pwd.h>
#include <cstring>

#define TOML_HEADER_ONLY 0
#define TOML_ENABLE_FORMATTERS 0
#include <toml++/toml.hpp>


namespace IRCat {
	enum DateFormat {
		dmy,
		mdy,
		ymd,
	};
	struct Server {
		std::string name;
		std::string ip_addr;
		std::vector<std::string> channels;
	};
	struct User {
		std::string nick;
		std::string real_name;
		std::string password;
		int status;
	};
	struct Options {
		bool vim_mode;
		DateFormat date_format;
		int year_digits;
		int time_format;
		std::vector<Server> servers;
		std::vector<User> Users;
		int default_server;
		int default_user;
	};
	class Config {
		private:
			std::fstream conf_file;
			toml::table conf_tbl;
		public:
			Config(std::string write);
			void write_options(Options options);
	};
}

#endif
