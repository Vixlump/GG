#include <iostream>
#include <sstream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <thread>
#include <chrono>
#include <cstdlib>
#include "ftxui/component/component.hpp"
// imported in gg.hpp: #include "ftxui/component/screen_interactive.hpp"

#include "xterm_colours.h"

#include "gg.hpp"

using namespace ftxui;

// Function to simulate frame callback
// todo make part of GG::Client::_frame_callback
// take this->_frame, this->_total_frames, and call this->_bmp_to_ascii(..., width, heigh)
std::string frame_callback(int width, int height) {
    static int counter = 0;
    std::ostringstream frame;
    frame << "Frame " << counter++ << "\n";
    for (int i = 0; i < height - 1; ++i) {
        for (int j = 0; j < width; ++j) {
            frame << ((i + j + counter) % 2 == 0 ? '#' : ' ');
        }
        frame << "\n";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(250)); // Simulate frame generation delay
    return frame.str();
}

void ShowMainMenu(ScreenInteractive& screen, GG::ScreenState& state);
void ShowDocumentationMenu(ScreenInteractive& screen, GG::ScreenState& state);
void ShowFunctionMenu(ScreenInteractive& screen, GG::ScreenState& state, int function_selected);
//void ShowVideoScreen(ScreenInteractive& screen, GG::ScreenState& state);

// Function to read frames from file
std::vector<std::string> ReadFramesFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> frames;
    std::string line;
    std::string frame;
    while (std::getline(file, line)) {
        if (line == "---FRAME---") {
            if (!frame.empty()) {
                frames.push_back(frame);
                frame.clear();
            }
        } else {
            frame += line + "\n";
        }
    }
    if (!frame.empty()) {
        frames.push_back(frame);
    }
    return frames;
}

// TODO: upgrade to c++20, b/c std::lerp has better numerical properties
float naive_lerp(float a, float b, float t) {
    return a + t * (b - a);
}

// right now this is 4 bit colour, not 8 bit colour.
void populate_color_lookup_table(CharFormat *color_lookup_table) {
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
				virtual_color_table[combined_rgb] = CharFormat {
					(uint8_t) fgi,
					(uint8_t) bgi,
					(uint8_t) chi
				};
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

// Function to play video
void GG::Client::_play_video(int fps) {
    CharFormat* colour_lookup_table = new CharFormat[16 * 16 * 16];
    populate_color_lookup_table(colour_lookup_table);
    
    auto screen = ScreenInteractive::Fullscreen();
    int frame_duration = 1000 / fps;

    bool playing = true;

    auto renderer = Renderer([&] {
        std::vector<Element> frame_elements;
        RenderableScreenBuffer buffer = this->_next_frame(80, 25, colour_lookup_table);
        std::string ascii_rep = buffer.char_buffer;

        size_t i = 0;
        while (ascii_rep.size() > 0) {
            int newline_pos = ascii_rep.find("\n");
            std::string line = ascii_rep.substr(0, newline_pos);

            // give each char in the matrix a unique colour
            std::vector<Element> row_elements;
            for (char ch : line) {
                std::string ch_str = std::string(1, ch);
                row_elements.push_back(text(ch_str) | color(buffer.fg_colour_buffer[i]) | bgcolor(buffer.bg_colour_buffer[i]));
                i++;
            }
            
            ascii_rep = ascii_rep.substr(newline_pos + 1);
            frame_elements.push_back(hbox(row_elements));
        }

        return vbox({
            text("Video Buffer:") | underlined | center,
            vbox(frame_elements) | border | center
        });
    });

    std::thread video_thread([&] {
        while (playing) {
            std::this_thread::sleep_for(std::chrono::milliseconds(frame_duration));
            screen.PostEvent(Event::Custom);
        }
    });

    screen.Loop(renderer);
    playing = false;
    if (video_thread.joinable()) {
        video_thread.join();
    }

    delete colour_lookup_table;
}

int GG::Client::_menu(GG::ScreenState state) {
    auto screen = ScreenInteractive::Fullscreen();
    //GG::ScreenState state = GG::ScreenState::MainMenu;

    while (state != GG::ScreenState::Exit) {
        switch (state) {
            case GG::ScreenState::MainMenu:
                ShowMainMenu(screen, state);
                break;
            case GG::ScreenState::DocumentationMenu:
                ShowDocumentationMenu(screen, state);
                break;
            case GG::ScreenState::FunctionMenu:
                // This case is handled within ShowDocumentationMenu, so it's not needed here.
                break;
            case GG::ScreenState::VideoScreen:
                this->_play_video(4);
                //this->_show_video_screen(screen, state);
                break;
            default:
                break;
        }
    }

  return 0;
}

void ShowMainMenu(ScreenInteractive& screen, GG::ScreenState& state) {
    std::vector<std::string> entries = {"Glegisterex", "Gloginob", "Glocumentationeb", "Play Video", "GLEXITEL"};
    int selected = 0;

    auto menu_option = MenuOption();
    menu_option.on_enter = [&] {
        if (entries[selected] == "Glegisterex") {
            std::cout << "Glegisterex" << std::endl;
        } else if (entries[selected] == "Gloginob") {
            // Handle login
        } else if (entries[selected] == "Glocumentationeb") {
            state = GG::ScreenState::DocumentationMenu;
            screen.ExitLoopClosure()();
        } else if (entries[selected] == "Play Video") {
            state = GG::ScreenState::VideoScreen;
            screen.ExitLoopClosure()();
        } else if (entries[selected] == "GLEXITEL") {
            state = GG::ScreenState::Exit;
            screen.ExitLoopClosure()();
        }
    };

    auto menu = Menu(&entries, &selected, menu_option);

    // Add the ASCII art to the menu renderer
    Component renderer = Renderer(menu, [&] {
        return vbox({
            text("o            o") | center,
            text(" \\          / ") | center,
            text("  \\        /  ") | center,
            text("    :-'""'-:    ") | center,
            text(" .-'  ____  `-. ") | center,
            text("( (  (_()_)  ) )") | center,
            text(" `-.   ^^   .-' ") | center,
            text("    `._==_.'    ") | center,
            text("    ___)(___    ") | center,
            text("Welcome to GLEEBLEGLOB!") | border | center | color(Color::CyanLight),
            menu->Render() | border,
        }) | center;
    });


    // Component renderer = Renderer(menu, [&] {
    //   return vbox({
    //     text("Welcome to GLEEBLEGLOB!") | border | center | color(Color::CyanLight),
    //     menu->Render() | border,
    //   }) | center;
    // });

    screen.Loop(renderer);
}

void ShowDocumentationMenu(ScreenInteractive& screen, GG::ScreenState& state) {
    std::vector<std::string> functions = {"Glegisterex", "Gloginob", "Glogoutob", "Gluploadem", "Glestreamy", "GLBACKEP"};

    int function_selected = 0;

    auto function_menu_option = MenuOption();
    function_menu_option.on_enter = [&] {
        if (functions[function_selected] != "GLBACKEP") {
            ShowFunctionMenu(screen, state, function_selected);
        } else {
            state = GG::ScreenState::MainMenu;
            screen.ExitLoopClosure()();
        }
    };

    auto function_menu = Menu(&functions, &function_selected, function_menu_option);

    Component function_renderer = Renderer(function_menu, [&] {
        return vbox({
            text("GLOCUMENTATIONEB!!!") | border | center | color(Color::CyanLight),
            function_menu->Render() | border,
        }) | center;
    });

    screen.Loop(function_renderer);
}

void ShowFunctionMenu(ScreenInteractive& screen, GG::ScreenState& state, int function_selected) {
    std::vector<std::string> functions = {"Glegisterex", "Gloginob", "Glogoutob", "Gluploadem", "Glestreamy", "GLBACKEP"};
    std::vector<std::string> syntax = {
        "gg register <username>", "gg login <options> <username>", "gg logout",
        "gg upload <options> <file>", "gg stream <options> <id>", "GLBACKEP"
    };
    std::vector<std::string> descriptions = {
        R"(
        - username: local username for this user
        - will prompt user for a password (this will be stored locally)
        - password is used to encrypt or hide the API key which will be used
        - will generate a locked "~/.gg<username>" file
        )",

        R"(
        - options:
        -> password: -p/--password password (string)
        - e.g.: gg login micahdbak
        - will attempt to unlock the ~/.gg<username> file and store the decrypted api key in ~/.ggsession
        - will prompt user for password if not provided
        )",

        R"(
        - e.g.: gg logout
        - will delete the ~/.ggsession file (this is not perfectly secure but stfu its fine)
        - will store any new uploads and their passwords in the ~/.gg<username> file
        )",

        R"(
        - options:
        -> upload from specific timestamps: -r/--range start:end (seconds:seconds) either when left blank assumes 0:end
        -> passkey to encrypt the file: -k/--key key (string)
        - file:
        -> some file to be uploaded to the server
        - e.g.: gg upload -k secret_file ./gleeby.mp4
        - will get session information from the ~/.ggsession file and then interact with the gg api
        - will use ffmpeg libraries to "chunk" the given file and encrypt it using the password
        - will prompt user for encryption key if not provided
        )",

        R"(
        - options:
        -> passkey to decrypt the chunks: -k/--key key (string)
        - will get session information from the ~/.ggsession file and then interact with the gg api
        - will prompt user for decryption key if not provided
        )"
    };


    auto function_name = Renderer([&] {
        return hbox({
            text("Function: ") | underlined,
            text(functions[function_selected]) | bgcolor(Color::CyanLight)
        }) | center;
    });

    auto function_syntax_title = text("Syntax: ") | underlined;
    auto function_syntax_content = text(syntax[function_selected]) | bold;
    auto function_syntax = Renderer([&] {
        return hbox({function_syntax_title, function_syntax_content}) | center;
    });

    auto function_description_title = text("Description: ") | underlined;
    auto function_description_content = text(descriptions[function_selected]);
    auto function_description = Renderer([&] {
        return vbox({function_description_title, text(descriptions[function_selected])}) | center;
    });

    // Custom button renderer to handle focus
    bool back_button_focused = false;
    auto back_button = Button("Back", [&] {
        state = GG::ScreenState::DocumentationMenu;
        screen.ExitLoopClosure()();
    });
    auto back_button_renderer = Renderer(back_button, [&] {
        auto style = back_button_focused ? (back_button->Render() | inverted | center) : (back_button->Render() | center);
        return style;
    });

    Component table = Container::Vertical({
        function_name,
        function_syntax,
        function_description,
        back_button_renderer
    });

    // Ensure the focus is managed
    auto container = Container::Vertical({
        table,
    });
    container->Add(back_button_renderer);
    container->SetActiveChild(0);

    // Screen loop to manage the UI and state
    screen.Loop(container);
}

void GG::Client::_show_video_screen(ScreenInteractive& screen, GG::ScreenState& state) {
    std::vector<std::string> frames = ReadFramesFromFile("message.txt");
    auto play_button = Button("Play Video", [&] {
        if (!frames.empty()) {
            this->_play_video(4); // 4 frames per second
        }
    });
    auto back_button = Button("Back", [&] {
        state = GG::ScreenState::MainMenu;
        screen.ExitLoopClosure()();
    });
    auto container = Container::Vertical({
        play_button,
        back_button
    });
    auto renderer = Renderer(container, [&] {
        return vbox({
            text("Welcome to Video Player!") | border | center | color(Color::CyanLight),
            play_button->Render() | center,
            back_button->Render() | center
        }) | center;
    });
    screen.Loop(renderer);
}
