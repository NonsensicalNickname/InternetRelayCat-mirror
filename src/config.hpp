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
#include <unordered_map>

#include "ftxui/dom/elements.hpp"    

#define TOML_HEADER_ONLY 0
#define TOML_ENABLE_FORMATTERS 0
#include <toml++/toml.hpp>

using namespace std::literals;
namespace fs = std::filesystem;

namespace IRCat {
	enum DateFormat {
		dmy,
		mdy,
		ymd,
	};
	enum Element {
		msg_origin,
		msg_body,
		bg,
		fg,
		highlighted,
		borders,
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
	class Config {
		private:
			std::fstream conf_file;
			toml::table conf_tbl;
			const char *conf_dir;
			ftxui::Color load_colour(std::string toml_el, toml::v3::node_view<toml::v3::node> *theme_tbl);
		public:
			Config();
			void write_options(); //replace to return toml++ error type
			int active_theme;
			//-----------------------------------
			bool vim_mode;
			DateFormat date_format;
			int year_digits;
			int time_format;
			int default_server;
			int default_user;
			int default_theme;
			Server server;
			std::vector<User> users;
			std::string theme_name;
			std::unordered_map<Element, ftxui::Color> theme;
	};
}

#endif
