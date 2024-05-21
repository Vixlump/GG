#include <iostream>
#include <string>

#include "gg.hpp"
#include "version.hpp"

std::string GG_HELP =
	"gg - "+GG::version+" - [g]leeble[g]lob command line interface.\n"+
	"Check out https://gleebleglob.club for service information.\n"+
	"GENERAL USAGE:\n"+
	"\tgg [command] [...arguments]\n"+
	"\tNOTE: excess arguments will be ignored.\n"+
	"COMMANDS:\n"+
	"\tgg help\n"+
	"\tgg version\n"+
	"\tgg register [username]\n"+
	"\tgg login [username]\n"+
	"\tgg logout\n"+
	"\tgg status\n"+
	"\tgg upload [file path] [password]\n"+
	"\tgg stream [file token] [password]";

std::string GG_INSUFFICIENT_ARGUMENTS= GG_HELP+"\n--\nInsufficient arguments.";

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
		if (argc != 3) {
			std::cout << GG_INSUFFICIENT_ARGUMENTS << std::endl;
			return 1;
		}

		std::string file_path = argv[2];
		return client.upload(file_path);
	// gg stream [file token] 
	} else if (cmd == "stream") {
		if (argc != 3) {
			std::cout << GG_INSUFFICIENT_ARGUMENTS << std::endl;
			return 1;
		}

		std::string file_token = argv[2];
		return client.stream(file_token);
	} else if (cmd == "version") {
		std::cout<<GG::version<<std::endl;
	} else {
		std::cout << GG_HELP << "\n--\nDon't know how to \"" << cmd << "\"." << std::endl;
		return 1;
	}

	// exec path shouldn't get here anyways
	return 0;
}
