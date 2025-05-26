#define TOML_IMPLEMENTATION
#include "config.hpp"

ftxui::Color IRCat::Config::load_colour(std::string toml_el, toml::v3::node_view<toml::v3::node> *theme_tbl) {
	auto *rgb_arr = theme_tbl->as_table()->at(toml_el).as_array();
	return ftxui::Color::RGB(rgb_arr->at(0).value_or(0),rgb_arr->at(1).value_or(0),rgb_arr->at(2).value_or(0));
}

IRCat::Config::Config() {

	 
	if (getenv("HOME") == 0) {
		conf_dir = std::string(getpwuid(getuid())->pw_dir);
	}
	else {
		conf_dir = std::string(getenv("HOME"));
	}
	conf_dir.append("/.config/ircat");
//	conf_dir = "/home/ceri/.config/ircat";

	struct stat buffer;	
	if (stat(conf_dir.c_str(), &buffer) != 0) {
		fs::create_directory(conf_dir);
		conf_dir.append("/config.toml");
		fs::copy_file("../config.toml", conf_dir);
	}
	else {
		conf_dir.append("/config.toml");
	}

	try {
		conf_tbl = toml::parse_file(conf_dir);
	}
	catch (const toml::parse_error &err) {
		std::cerr << err;
	}

	default_server  = conf_tbl["login"]["default_server"].value_or(0);
	default_user = conf_tbl["login"]["default_user"].value_or(0);
	default_theme = conf_tbl["login"]["default_theme"].value_or(0);
	
	auto server_tbl = conf_tbl["servers"][default_server];

	server.name = server_tbl["name"].value_or(""sv);
	server.ip_addr = server_tbl["ip"].value_or(""sv);
	if (toml::array *server_channels = server_tbl["channels"].as_array()) {
		for (auto it = server_channels->begin(); it != server_channels->end(); ++it) {
			auto i = std::distance(server_channels->begin(), it); 
			server.channels.push_back(server_channels->at(i).value_or(""));
		}
	}

	if (toml::array *users_arr = conf_tbl["users"].as_array()) {
		for (auto it = users_arr ->begin(); it != users_arr ->end(); ++it) {
			auto i = std::distance(users_arr ->begin(), it); 
			toml::table *user_tbl = users_arr->at(i).as_table();
			IRCat::User user;
			user.nick = user_tbl->at("nick").value_or(""sv);
			user.real_name = user_tbl->at("name").value_or(""sv);
			user.password = user_tbl->at("pass").value_or(""sv);
			user.status = user_tbl->at("default_status").value_or(8);
			users.push_back(user);
		}
	}

	auto theme_tbl = conf_tbl["themes"][default_theme];

	theme_name = theme_tbl["name"].value_or(""sv);
	theme[IRCat::highlighted] = load_colour("highlighted", &theme_tbl);
	theme[IRCat::msg_origin] = load_colour("msg_origin", &theme_tbl);
	theme[IRCat::msg_body] = load_colour("msg_body", &theme_tbl);
	theme[IRCat::borders] = load_colour("borders", &theme_tbl);
	theme[IRCat::bg] = load_colour("bg", &theme_tbl);
	theme[IRCat::fg] = load_colour("fg", &theme_tbl);

	vim_mode = conf_tbl["vim_mode"].value_or(false);

	//conf_file.open(conf_fpath.append("/config.toml"), std::ios::out);
	//conf_file.close();
	/*
	vim_mode;
	date_format;
	year_digits;
	time_format;
	servers;
	users;
	default_server;
	default_user;
	theme;
	*/
}

