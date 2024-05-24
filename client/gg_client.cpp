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

	// for tput see https://pubs.opengroup.org/onlinepubs/9699919799.2018edition/utilities/tput.html
	long num_colours = std::strtol(exec("tput colors").c_str(), nullptr, 10);
	this->_terminal_supports_colour = (num_colours >= 8);
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

// right now this is 4 bit colour, not 8 bit colour.
void GG::Client::_populate_color_lookup_table(CharFormat *color_lookup_table, ColorSchemeKind kind) {
	// for char densities see https://stackoverflow.com/questions/30097953/ascii-art-sorting-an-array-of-ascii-characters-by-brightness-levels-c-c
	const size_t NUM_CHARS = 12;
	const float densities[NUM_CHARS] = {
		0.0, 0.0829, 0.0848, 0.1403, 0.2417, 0.2919, 0.3294, 0.4580, 0.5972, 0.6809, 0.7834, 0.9999
	};

	// this number converts between a [0,1] range density, and a value that represents the fraction of the viewport that's coloured, by division.
	// It's an approximate that I got by measuring the number of unaffected pixels in a character rendered in Consolas. 
	const float CONVERSION_FACTOR = 1.742514;

	std::unordered_map<uint32_t, CharFormat> virtual_color_table;
	for (size_t fgi = 0; fgi < NUM_XTERM_COLOURS; fgi++) {
		int fg_r = (xterm_colours[fgi] & 0xff0000) >> 16;
		int fg_g = (xterm_colours[fgi] & 0x00ff00) >> 8;
		int fg_b = xterm_colours[fgi] & 0x0000ff;

		for (size_t bgi = 0; bgi < NUM_XTERM_COLOURS; bgi++) {
			int bg_r = (xterm_colours[bgi] & 0xff0000) >> 16;
			int bg_g = (xterm_colours[bgi] & 0x00ff00) >> 8;
			int bg_b = xterm_colours[bgi] & 0x0000ff;

			for (size_t chi = 0; chi < NUM_CHARS; chi++) {
				int iresult_r = std::lround(naive_lerp((float)bg_r, (float)fg_r, densities[chi] / CONVERSION_FACTOR));
				int iresult_g = std::lround(naive_lerp((float)bg_g, (float)fg_g, densities[chi] / CONVERSION_FACTOR));
				int iresult_b = std::lround(naive_lerp((float)bg_b, (float)fg_b, densities[chi] / CONVERSION_FACTOR));

                // 0xf0 converts from 8 bit colours to 4 bit colours (ignore the less significant 4 bits)
				uint8_t result_r = 0xf0 & (iresult_r > 255 ? 255 : (uint8_t) iresult_r);
				uint8_t result_g = 0xf0 & (iresult_g > 255 ? 255 : (uint8_t) iresult_g);
				uint8_t result_b = 0xf0 & (iresult_b > 255 ? 255 : (uint8_t) iresult_b);

				uint32_t combined_rgb = (result_r << 16) | (result_g << 8) | result_b;

				auto new_format = CharFormat {
					(uint8_t) fgi,
					(uint8_t) bgi,
					(uint8_t) chi
				};
				if (virtual_color_table.find(combined_rgb) != virtual_color_table.end()) {
					auto existing_format = virtual_color_table[combined_rgb];
					if (kind == ColorSchemeKind::Contrast) {
						int new_contrast = get_fg_bg_contrast(
							xterm_colours[new_format.fg_color_index], 
							xterm_colours[new_format.bg_color_index]
						);
						int existing_contrast = get_fg_bg_contrast(
							xterm_colours[existing_format.fg_color_index], 
							xterm_colours[existing_format.bg_color_index]
						);
						if (new_contrast < existing_contrast) {
							virtual_color_table[combined_rgb] = new_format;
						}

					} else if (kind == ColorSchemeKind::MSE) {
						int new_mse = get_fg_bg_mse(
							xterm_colours[new_format.fg_color_index], 
							xterm_colours[new_format.bg_color_index]
						);
						int existing_mse = get_fg_bg_mse(
							xterm_colours[existing_format.fg_color_index], 
							xterm_colours[existing_format.bg_color_index]
						);
						if (new_mse < existing_mse) {
							virtual_color_table[combined_rgb] = new_format;
						}

					} else {
						virtual_color_table[combined_rgb] = new_format;
					}

				} else {
					virtual_color_table[combined_rgb] = new_format;
				}
				
			}
		}
	}

	printf("virtual_color_table size: %lu\n", virtual_color_table.size());

	// TODO: find closest match in all of virtual_color_table's keys
    // We'll have to sort the virtual colour table by intensity, then use upper & lower bounds to search a range. 
    // Ex: if our closest match has a distance of 16, then all intensity distances larger than 16 can be ignored.

    for (auto& it: virtual_color_table) {
        // each of r,g,b are 4 bit integers (at least, pretend they are)
		int r = (it.first & 0xf00000) >> (16 + 4);
		int g = (it.first & 0x00f000) >> (8 + 4);
		int b = (it.first & 0x0000f0) >> (0 + 4);
        color_lookup_table[r * 16 * 16 + g * 16 + b] = it.second;
    }
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

	// create colour lookup table for generating ascii colour codes 
	CharFormat* colour_lookup_table = new CharFormat[16 * 16 * 16];
    this->_populate_color_lookup_table(colour_lookup_table, ColorSchemeKind::MSE);

	int frame = 1;
	int last_num_rows = -1;

	// display frames (wait for SIGKILL)
	while (true) {
		auto frame_start = std::chrono::high_resolution_clock::now();

		std::string fname = this->_bmp_dir + "/" + std::to_string(frame) + ".bmp";
		struct winsize w;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

		int curr_num_rows = w.ws_row - 1;
		int curr_num_cols = w.ws_col - 1;
		if (last_num_rows > 0) {
			// move cursor to top of the screen
			// see: https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
			std::cout << repeat(std::string("\033M"), last_num_rows);
		}

		std::cout << GG::bmp_to_ascii(fname.c_str(), curr_num_cols, curr_num_rows, this->_terminal_supports_colour, colour_lookup_table);
		last_num_rows = curr_num_rows;
		fflush(stdout); // flush the screen

		auto frame_end = std::chrono::high_resolution_clock::now();

		const int FPS = 8;
		auto elapsed_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(frame_end - frame_start);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FPS) - elapsed_duration);

		// elapses worst case 836405 ns on my machine at full-full screen
		//debug_log("elapsed_duration: " + std::to_string(elapsed_duration.count()) + " ns\n");

		frame++;

		// loop the video
		if (frame > num_bmps) {
			frame = 1;
		}
	}

	return 0;
}
