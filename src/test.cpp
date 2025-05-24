#include <iostream>
#include "config.cpp"
#include "config.hpp"

int main() {
	IRCat::Config conf = IRCat::Config();
	for (struct IRCat::User user: conf.users) {
		std::cout << user.nick;
	}
	std::cout << conf.default_user;
}
