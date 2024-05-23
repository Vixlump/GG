#ifndef GG_HPP
#define GG_HPP

#include <cstdint>
#include <fstream>
#include <string>
#include <unordered_map>

#include "ftxui/component/screen_interactive.hpp"

inline void debug_log(std::string contents) {
	std::ofstream file;
	file.open("log.txt", std::ios_base::app);
	file << contents;
	file.close();
}

// the formatting needed to describe a single unit of the terminal screen. 
// "a pixel", but not. "a character", but not. both!
struct CharFormat {
	uint8_t fg_color_index; // this is an index into the xterm_colours array
	uint8_t bg_color_index;
	uint8_t char_index;
};

// contains all the buffers needed to render a screen.
struct RenderableScreenBuffer {
	std::string char_buffer;
	std::vector<ftxui::Color> fg_colour_buffer;
	std::vector<ftxui::Color> bg_colour_buffer;
};

namespace GG {	
	enum class ScreenState {
		MainMenu,
		DocumentationMenu,
		FunctionMenu,
		VideoScreen,
		Exit
	};

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

		// for rendering coloured ascii :)
		bool _terminal_supports_colour;

		// the menu
		int _menu(ScreenState state);

		// video player
		void _play_video(int fps);

		// saarang moment
		void _show_video_screen(ftxui::ScreenInteractive& screen, ScreenState& state);

		// load session information from a file
		void _load_session(std::string file_path);

		// save session information to a file
		void _save_session(std::string file_path);

		// clear session information from a file
		void _clear_session(std::string file_path);

		// get the next frame given terminal width and height
		RenderableScreenBuffer _next_frame(int term_width, int term_height, CharFormat *colour_lookup_table);

		// helper for getting unix timestamp
		std::string _unix_timestamp();
	};
	
	// helper functions
	RenderableScreenBuffer bmp_to_ascii(
		const char *file_path,
		int w,
		int h,
		bool enable_colour,
		CharFormat *colour_lookup_table
	);
	bool upload_file(std::string file_name);
	void receive_file(int serverSocket, std::string & filetoken);
	int download_file(std::string filetoken);
	std::string network_custom(std::string custom_network_message);
}

#endif
