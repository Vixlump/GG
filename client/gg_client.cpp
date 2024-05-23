#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <utility>

#include <dirent.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <unistd.h>

#include "gg.hpp"

std::string GG::Client::_unix_timestamp() {
	int unix_epoch = time(NULL);
	return std::to_string(unix_epoch);
}

GG::Client::Client() {
	const char *home = getenv("HOME");
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
	// make sure no past user data is loaded
	this->logout();

	this->_user = user;
	this->_api_key = "unused";

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

	ret = "Logged in as \"" + this->_user + "\".";

	if (this->_itop.size() == 0) {
		return ret; // early return
	}

	ret += "\nHistory (Unordered):";

	// iterate over the identifier-to-password map
	for (auto it = this->_itop.begin(); it != this->_itop.end(); ++it) {
		ret += "\n* " + it->first;
	}

	return ret;
}

int GG::Client::upload(std::string file_path) {
	struct stat st;

	// check if /tmp/gg doesn't exist
	if (stat("/tmp/gg", &st) != 0) {
		// make it
		mkdir("/tmp/gg", S_IFDIR|S_IRWXU|S_IRWXG|S_IRWXO);
	}

	this->_bmp_dir = "/tmp/gg/" + this->_unix_timestamp();
	// make the directory for the bmps
	mkdir(this->_bmp_dir.c_str(), S_IFDIR|S_IRWXU|S_IRWXG|S_IRWXO);

	std::string command = "ffmpeg -hide_banner -loglevel error -i " + file_path + " -vf \"scale=200:100,fps=8\" " + _bmp_dir + "/%d.bmp";
	int ret = system(command.c_str());

	// check for errors
	if (ret != 0) {
		// exit early
		return ret;
	}

	// run sys command tar-balling all bmps in /tmp/gg/<unix timestamp>/*.bmp
	std::string tarball_path = "/tmp/gg/" + this->_unix_timestamp() + ".tar";
	command = "tar -cf " + tarball_path + " " + this->_bmp_dir + "/*.bmp";
	ret = system(command.c_str());

	if (ret != 0)  {
		return ret;
	}

	// open connection with server (using api key) and transfer the tarball to the server
	if (!GG::upload_file(tarball_path)) {
		std::cerr << "Error uploading .tar file to the server" << std::endl;
		return -1;
	}

	std::string token = GG::network_custom("get_v");
	this->_itop.insert(std::make_pair(token, "unused"));

	std::cout << "Uploaded! Here's your stream token: " << token << '\n';

	return 0; // success
}

int GG::Client::stream(std::string token) {
	// check if this token is in the history yet
	if (this->_itop.count(token) == 0) {
		this->_itop.insert(std::make_pair(token, "unused"));
	}

	struct stat st;

	// check if the tmp dir exists
	if (stat("/tmp/gg", &st) != 0) {
		// directory doesn't exist
		mkdir("/tmp/gg", S_IFDIR|S_IRWXU|S_IRWXG|S_IRWXO);
	}

	std::cout << token << std::endl;
	this->_bmp_dir = "/tmp/gg/" + token;

	if (stat(this->_bmp_dir.c_str(), &st) != 0) {
		// haven't streamed this yet; download the frames
		if (GG::download_file(token) != 0) {
			std::cerr << "Error downloading file from the server" << std::endl;
			return -1;
		}

		// create the necessary directory
		mkdir(this->_bmp_dir.c_str(), S_IFDIR|S_IRWXU|S_IRWXG|S_IRWXO);

		// extract the files from the tarball into /tmp/gg/{token}
		std::string command = "tar -xf /tmp/gg/" + token + ".tar --strip-components 3 -C " + this->_bmp_dir;
		int ret = system(command.c_str());

		// check that unzipping was successful
		if (ret != 0) {
			return ret;
		}
	}

	// open the directory of bmps
	DIR *dir = opendir(this->_bmp_dir.c_str());
	struct dirent *entry;
	int num_bmps = 0;

	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_name[0] == '.') {
			continue; // these aren't BMPs
		}

		num_bmps++;
	}

	closedir(dir);
	dir = NULL;

	int frame = 1;
	
	// display frames (wait for SIGKILL)
	while (true) {
		std::string fname = this->_bmp_dir + "/" + std::to_string(frame) + ".bmp";
		struct winsize w;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

		std::cout << GG::bmp_to_ascii(fname.c_str(), w.ws_col - 1, w.ws_row - 1);
		fflush(stdout); // flush the screen

		std::this_thread::sleep_for(std::chrono::milliseconds(125)); // 8fps

		frame++;

		// loop the video
		if (frame > num_bmps) {
			frame = 1;
		}

	}

	return 0;
}
