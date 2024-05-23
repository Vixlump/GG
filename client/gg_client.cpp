#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <cstdlib>

// lovely C libraries
#include <dirent.h>
#include <sys/stat.h>
#include <SDL2/SDL.h>
#include <time.h>

#include "ftxui/component/screen_interactive.hpp"

#include "gg.hpp"

// for getting stdout from terminal commands
std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

std::string GG::Client::_unix_timestamp() {
	int unix_epoch = time(NULL);
	return std::to_string(unix_epoch);
}

GG::Client::Client() {
	char *home = getenv("HOME");
	this->_home = home == NULL ? "." : home;
	this->_load_session(this->_home + "/.ggsession");

	// for tput see https://pubs.opengroup.org/onlinepubs/9699919799.2018edition/utilities/tput.html
	//long num_colours = std::strtol(exec("tput colors").c_str(), nullptr, 10);
	//this->_terminal_supports_colour = (num_colours >= 8);
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
		size_t colon_index = line.find(":");

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

//	this->_menu();

	return ret;
}

int GG::Client::upload(std::string file_path) {
	std::string dir_general_path = "/tmp/gg/";
	struct stat st;

	if (stat(dir_general_path.c_str(), &st) != 0) {
		// directory doesn't exist
		mkdir(dir_general_path.c_str(), S_IFDIR|S_IRWXU|S_IRWXG|S_IRWXO);
	}

	_dir_path = dir_general_path;
	std::string unix_timestamp = this->_unix_timestamp();
	_dir_path += unix_timestamp;
	mkdir(_dir_path.c_str(), S_IFDIR|S_IRWXU|S_IRWXG|S_IRWXO);

	std::string command = "ffmpeg -hide_banner -loglevel error -i ";
	command += file_path;
	command += " -vf \"scale=400:200,fps=4\" ";
	command += _dir_path;
	command += "/%d.bmp";

	int ret = system(command.c_str());
	if (ret != 0) {
		std::cout << "Error while running ffmpeg." << std::endl;
		return ret;
	}

	// run sys command tar-balling all bmps in /tmp/gg/<unix timestamp>/*.bmp
	command.clear();
	command = "tar -cf ";
	command += "/tmp/gg/";
	command += unix_timestamp + ".tar";
	command += " /tmp/gg/";
	command += unix_timestamp;
	command += "/*.bmp";

	std::string tarball_path = "/tmp/gg/" + unix_timestamp + ".tar";

	ret = system(command.c_str());
	if (ret != 0) 
	{
		std::cerr << "Error tarballing the .bmp files" << std::endl;
		return ret;
	}

	// open connection with server (using api key) and transfer the tarball to the server
	// call it a day
	if (!GG::upload_file(tarball_path))
	{
		std::cerr << "Error uploading .tar file to the server" << std::endl;
		return -1;
	}

	std::string tmp = GG::network_custom("get_v");

	this->_itop.insert(std::make_pair(tmp, "1234 (highly secure, don't hack us :/)"));

	std::cout << "File Token: " << tmp << '\n';

	return 0;
}

int GG::Client::stream(std::string token) {
	struct stat st;

	if (this->_itop.count(token) == 0) {
		this->_itop.insert(std::make_pair(token, "1234 (highly secure, don't hack us :/)"));
	}

	if (stat("/tmp/gg/", &st) != 0) {
		// directory doesn't exist
		mkdir("/tmp/gg/", S_IFDIR|S_IRWXU|S_IRWXG|S_IRWXO);
	}

	std::string unix_timestamp = this->_unix_timestamp();

	if (GG::download_file(token) != 0)
	{
		std::cerr << "Error downloading file from the server" << std::endl;
		return -1;
	}

	this->_dir_path = "/tmp/gg/" + unix_timestamp;
	mkdir(this->_dir_path.c_str(), S_IFDIR|S_IRWXU|S_IRWXG|S_IRWXO);

	// extract the files from the tarball into /tmp/gg/{unix_timestamp}
	// tar -xvzf community_images.tar format
	std::string command = "tar -xf /tmp/gg/";
	command += token;
	command += ".tar --strip-components 3 -C " + this->_dir_path;

	int ret = system(command.c_str());
	if (ret != 0) 
	{
		std::cerr << "Error extracting the tarball" << std::endl;
		return ret;
	}
	
	// open the directory of bmps
	DIR *dir = opendir(this->_dir_path.c_str());
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
	
	// initialize frame counts
	this->_frame = 1;
	this->_total_frames = num_bmps;

	// this displays the menu
	//return this->_menu(ScreenState::VideoScreen);

	this->_play_video(4); // 4fps

	std::cout << "cleaning up" << std::endl;

	return 0;
}

RenderableScreenBuffer GG::Client::_next_frame(int term_width, int term_height, CharFormat *colour_lookup_table) {
	if (this->_frame >= this->_total_frames) {
		this->_frame = 1;
	}

	std::string fname = this->_dir_path + "/" + std::to_string(this->_frame) + ".bmp";
	RenderableScreenBuffer buffer = GG::bmp_to_ascii(
		fname.c_str(), 
		term_width, 
		term_height, 
		this->_terminal_supports_colour, 
		colour_lookup_table
	);
	this->_frame++;

	return buffer;
}
