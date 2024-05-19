#ifndef GG_HPP
#define GG_HPP

#include <string>
#include <unordered_map>

namespace GG {
	class Client {
	public:
		Client();
		~Client();

		// request an api key from the server
		int request(std::string user);
		// log in to a given user (opens ~/.gg{user})
		int login(std::string user);
		// logs out from the current session (deletes ~/.ggsession)
		int logout(void);
		// returns a string describing the current status
		std::string status(void);
		// uploads a video located at the given path encrypting with the given password
		int upload(std::string file_path);
		// downloads a video from the server using the token, then streams it
		// (either to localhost:3000 or an ASCII-ify'd form)
		int stream(std::string token);
	private:
		// home directory (gets from $HOME)
		std::string _home;
		// the current user name (for ~/.gg{user})
		std::string _user;
		// the api key with which to interact with the server
		std::string _api_key;
		// the path to directory containing BMPs
		std::string _dir_path;
		// current frame (in _dir_path) and total frames
		int _frame, _total_frames;
		// maps identifier (string) to password (string)
		std::unordered_map<std::string, std::string> _itop;

		// load session information from a file
		void _load_session(std::string file_path);

		// save session information to a file
		void _save_session(std::string file_path);

		// clear session information from a file
		void _clear_session(std::string file_path);

		// get the next frame given terminal width and height
		std::string _next_frame(int term_width, int term_height);

		// helper for getting unix timestamp
		std::string _unix_timestamp();
	};

	// helper functions
	std::string bmp_to_ascii(const char *file_path, int w, int h);
	bool upload_file(std::string file_name);
	void receive_file(int serverSocket, std::string & filetoken);
	int download_file(std::string filetoken);
	std::string network_custom(std::string custom_network_message);
}

#endif
