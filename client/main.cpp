#include <iostream>
#include <string>

#include "gg.hpp"

#define GG_HELP\
	"gg - v0.1 - [g]leeble[g]lob command line interface.\n"\
	"Check out https://gleebleglob.club for service information.\n"\
	"GENERAL USAGE:\n"\
	"\tgg [command] [...arguments]\n"\
	"\tNOTE: excess arguments will be ignored.\n"\
	"COMMANDS:\n"\
	"\tgg help\n"\
	"\tgg register [username]\n"\
	"\tgg login [username]\n"\
	"\tgg logout\n"\
	"\tgg status\n"\
	"\tgg upload [file path] [password]\n"\
	"\tgg stream [file token] [password]"

#define GG_INSUFFICIENT_ARGUMENTS  GG_HELP "\n--\nInsufficient arguments."

auto main(int argc, char **argv) -> int {
	// if no arguments provided, assume 'help' command
	if (argc == 1) {
		std::cout << GG_HELP << std::endl;
		return 1;
	}

	// gleebleglob client
	GG::Client client;

	// parse command from arguments
	std::string cmd = argv[1];

	// help command
	if (cmd == "help") {
		std::cout << GG_HELP << std::endl;
		return 0;
	// gg register [username]
	} else if (cmd == "register") {
		if (argc != 3) {
			std::cout << GG_INSUFFICIENT_ARGUMENTS << std::endl;
			return 1;
		}

		std::string username = argv[2];
		return client.request(username);
	// gg login [username]
	} else if (cmd == "login") {
		if (argc != 3) {
			std::cout << GG_INSUFFICIENT_ARGUMENTS << std::endl;
			return 1;
		}

		std::string username = argv[2];
		return client.login(username);
	// gg logout
	} else if (cmd == "logout") {
		return client.logout();
	// gg status
	} else if (cmd == "status") {
		std::cout << client.status() << std::endl;
		return 0;
	// gg upload [file path] [password]
	} else if (cmd == "upload") {
		if (argc != 4) {
			std::cout << GG_INSUFFICIENT_ARGUMENTS << std::endl;
			return 1;
		}

		std::string file_path = argv[2];
		std::string password = argv[3];
		return client.upload(file_path, password);
	// gg stream [file token] [password]
	} else if (cmd == "stream") {
		if (argc != 4) {
			std::cout << GG_INSUFFICIENT_ARGUMENTS << std::endl;
			return 1;
		}

		std::string file_token = argv[2];
		std::string password = argv[3];
		return client.stream(file_token, password);
	} else {
		std::cout << GG_HELP << "\n--\nDon't know how to \"" << cmd << "\"." << std::endl;
		return 1;
	}

	// exec path shouldn't get here anyways
	return 0;
}
