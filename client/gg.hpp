#ifndef GG_HPP
#define GG_HPP

#include <string>
#include <unordered_map>

namespace GG {
	class client {
	public:
		client();
		~client();

		// request an api key from the server
		int request(std::string user);
		// log in to a given user (opens ~/.gg{user})
		int login(std::string user);
		// logs out from the current session (deletes ~/.ggsession)
		int logout(void);
		// returns a string describing the current status
		std::string status(void);
		// uploads a video located at the given path encrypting with the given password
		int upload(std::string path, std::string password);
		// downloads a video from the server using the token, then streams it
		// (either to localhost:3000 or an ASCII-ify'd form)
		int stream(std::string token, std::string password);
	private:
		// home directory (gets from $HOME)
		std::string _home;
		// the current user name (for ~/.gg{user})
		std::string _user;
		// the api key with which to interact with the server
		std::string _api_key;
		// maps identifier (string) to password (string)
		std::unordered_map<std::string, std::string> _itop;

		// load session information from a file
		void _load_session(std::string file_path);

		// save session information to a file
		void _save_session(std::string file_path);

		// clear session information from a file
		void _clear_session(std::string file_path);
	};
}

#endif
