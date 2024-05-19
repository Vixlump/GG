#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <cstdlib>

#include "gg.hpp"

GG::Client::Client() {
	char *home = getenv("HOME");
	this->_home = home == NULL ? "." : home;
	this->_load_session(this->_home + "/.ggsession");
}

GG::Client::~Client() {
	if (this->_user.size() > 0) {
		this->_save_session(this->_home + "/.ggsession");
	}
}

void GG::Client::_load_session(std::string file_path) {
	std::ifstream session_file(file_path, std::ifstream::in);

	if (!session_file.is_open()) {
		return; // couldn't open file
	}
	// session file is open; read from it

	// clear the identifier to password map
	this->_itop.clear();

	std::string line;

	// read every line from the file as lines in the form "key:value"
	while (session_file.good()) {
		std::getline(session_file, line);
		int colon_index = line.find(":");

		if (colon_index == std::string::npos) {
			continue; // bad line
		}

		std::string key = line.substr(0, colon_index);
		std::string value = line.substr(colon_index + 1, std::string::npos);

		if (key == "user") {
			this->_user = value; // set the username
		} else if (key == "api_key") {
			this->_api_key = value; // set the api key
		} else {
			// set some identifier-to-password pair
			this->_itop.insert(std::make_pair(key, value));
		}
	}

	session_file.close();
}

void GG::Client::_save_session(std::string file_path) {
	std::ofstream session_file(file_path, std::ofstream::out);

	if (!session_file.is_open()) {
		std::cout
			<< "\n--\nError: Couldn't write to session file\n"
			<< "Check write permissions to ~ and ~/.ggsession"
			<< std::endl;
		return;
	}

	session_file << "user:" << this->_user << "\n";
	session_file << "api_key:" << this->_api_key << "\n";

	// iterate over the identifier-to-password map
	for (auto it = this->_itop.begin(); it != this->_itop.end(); ++it) {
		// write a line to the session file
		session_file << it->first << ":" << it->second << "\n";
	}

	session_file.close();
}

void GG::Client::_clear_session(std::string file_path) {
	std::ofstream session_file(file_path, std::ofstream::out);
	session_file.close(); // should write the file as empty
}

int GG::Client::request(std::string user) {
	this->_user = user;
	this->_api_key = "this_is_a_fake_api_key";

	this->_save_session(this->_home + "/.gg" + user);

	// output current status
	std::cout << "Registered and logged in as " << user << "." << std::endl;

	return 0;
}

int GG::Client::login(std::string user) {
	this->logout(); // log out any user before logging in

	// attempt to load the session stored in ~/.gg{user}
	this->_load_session(this->_home + "/.gg" + user);

	// output current status
	std::cout << this->status() << std::endl;

	return 0;
}

int GG::Client::logout(void) {
	// check if currently logged in
	if (this->_user.size() > 0) {
		// attempt to save the current session to ~/.gg{user}
		this->_save_session(this->_home + "/.gg" + this->_user);
		this->_clear_session(this->_home + "/.ggsession");

		// clear fields
		this->_user = "";
		this->_api_key = "";
		this->_itop.clear();
	}

	return 0;
}

std::string GG::Client::status(void) {
	std::string ret;

	if (this->_user.size() == 0) {
		ret += "Not logged in.";
		return ret;
	}

	ret += "Logged in as \"";
	ret += this->_user;
	ret += "\".";

	if (this->_itop.size() == 0) {
		return ret; // early return
	}

	ret += "\nCan stream ";

	// iterate over the identifier-to-password map
	auto it = this->_itop.begin();

	if (it != this->_itop.end()) {
		ret += it->first;
		it++;

		if (it == this->_itop.end()) {
			ret += ".";
			return ret;
		}
	}

	// there is more identifiers in map
	while (true) {
		std::string identifier = it->first;
		it++;

		if (it != this->_itop.end()) {
			ret += ", ";
			ret += identifier;
		} else {
			ret += ", and ";
			ret += identifier;
			ret += ".";
			break; // close while loop
		}
	}

	return ret;
}

int GG::Client::upload(std::string file_path, std::string pass) {
	std::cout
		<< "upload\n"
		<< "file_path: " << file_path << "\n"
		<< "pass: " << pass << std::endl;

	return 0;
}

int GG::Client::stream(std::string token, std::string pass) {
	std::cout
		<< "stream\n"
		<< "token: " << token << "\n"
		<< "pass: " << pass << std::endl;

	return 0;
}
