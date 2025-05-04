#define TOML_IMPLEMENTATION
#include "config.hpp"

using namespace std::literals;
namespace fs = std::filesystem;
const char *conf_dir;

IRCat::Config::Config(std::string write) {

	if (getenv("HOME") == 0) {
		conf_dir = strcat(getpwuid(getuid())->pw_dir, "/.config/ircat");
	}
	else {
		conf_dir = strcat(getenv("HOME"), "/.config/ircat");
	}
	std::string conf_fpath(conf_dir);
	conf_fpath.append("/config.toml");

	struct stat buffer;	
	if (stat(conf_dir, &buffer) != 0) {
		fs::create_directory(conf_dir);
		fs::copy_file("../config.toml", conf_fpath);
	}

	conf_tbl = toml::parse_file(conf_fpath);

	int login_server = conf_tbl["login"]["default_server"].value_or(0);
	int login_user = conf_tbl["login"]["default_user"].value_or(0);
	std::string_view sname = conf_tbl["servers"][login_server]["name"].value_or(""sv);
	std::string_view uname = conf_tbl["users"][login_server]["nick"].value_or(""sv);
	IRCat::Server server;
	server.name = sname;
	std::cout << server.name << "\n" << uname;
	//conf_file.open(conf_fpath.append("/config.toml"), std::ios::out);
	//conf_file.close();
}
