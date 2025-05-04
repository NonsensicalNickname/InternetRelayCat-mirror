#include <iostream>
#include "config.cpp"

int main() {
	std::string st = "wahoo!!!\n";
	IRCat::Config conf_file = IRCat::Config(st);
}
