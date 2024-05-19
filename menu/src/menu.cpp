#include <iostream>
#include <sstream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

enum class ScreenState {
  MainMenu,
  DocumentationMenu,
  FunctionMenu,
  VideoScreen,
  Exit
};

// Function to simulate frame callback
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

void ShowMainMenu(ScreenInteractive& screen, ScreenState& state);
void ShowDocumentationMenu(ScreenInteractive& screen, ScreenState& state);
void ShowFunctionMenu(ScreenInteractive& screen, ScreenState& state, int function_selected);
void ShowVideoScreen(ScreenInteractive& screen, ScreenState& state);

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

// Function to play video
void PlayVideo(ScreenInteractive& screen, const std::vector<std::string>& frames, int fps) {
    int frame_duration = 1000 / fps;

    bool playing = true;
    int current_frame = 0;

    auto renderer = Renderer([&] {
        std::vector<Element> frame_elements;
        std::istringstream frame_stream(frames[current_frame]);
        std::string line;
        while (std::getline(frame_stream, line)) {
            frame_elements.push_back(text(line));
        }

        return vbox({
            text("Video Buffer:") | underlined | center,
            vbox(frame_elements) | border | center
        });
    });

    std::thread video_thread([&] {
        while (playing) {
            std::this_thread::sleep_for(std::chrono::milliseconds(frame_duration));
            current_frame = (current_frame + 1) % frames.size();
            screen.PostEvent(Event::Custom);
        }
    });

    screen.Loop(renderer);
    playing = false;
    if (video_thread.joinable()) {
        video_thread.join();
    }
}

int main() {
  auto screen = ScreenInteractive::Fullscreen();
  ScreenState state = ScreenState::MainMenu;

  while (state != ScreenState::Exit) {
    switch (state) {
      case ScreenState::MainMenu:
        ShowMainMenu(screen, state);
        break;
      case ScreenState::DocumentationMenu:
        ShowDocumentationMenu(screen, state);
        break;
      case ScreenState::FunctionMenu:
        // This case is handled within ShowDocumentationMenu, so it's not needed here.
        break;
      case ScreenState::VideoScreen:
        ShowVideoScreen(screen, state);
        break;
      default:
        break;
    }
  }

  return 0;
}

void ShowMainMenu(ScreenInteractive& screen, ScreenState& state) {
  std::vector<std::string> entries = {"Glegisterex", "Gloginob", "Glocumentationeb", "Play Video", "GLEXITEL"};
  int selected = 0;

  auto menu_option = MenuOption();
  menu_option.on_enter = [&] {
    if (entries[selected] == "Glegisterex") {
      std::cout << "Glegisterex" << std::endl;
    } else if (entries[selected] == "Gloginob") {
      // Handle login
    } else if (entries[selected] == "Glocumentationeb") {
      state = ScreenState::DocumentationMenu;
      screen.ExitLoopClosure()();
    } else if (entries[selected] == "Play Video") {
      state = ScreenState::VideoScreen;
      screen.ExitLoopClosure()();
    } else if (entries[selected] == "GLEXITEL") {
      state = ScreenState::Exit;
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

void ShowDocumentationMenu(ScreenInteractive& screen, ScreenState& state) {
  std::vector<std::string> functions = {"Glegisterex", "Gloginob", "Glogoutob", "Gluploadem", "Glestreamy", "GLBACKEP"};

  int function_selected = 0;

  auto function_menu_option = MenuOption();
  function_menu_option.on_enter = [&] {
    if (functions[function_selected] != "GLBACKEP") {
      ShowFunctionMenu(screen, state, function_selected);
    } else {
      state = ScreenState::MainMenu;
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

void ShowFunctionMenu(ScreenInteractive& screen, ScreenState& state, int function_selected) {
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
    state = ScreenState::DocumentationMenu;
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

void ShowVideoScreen(ScreenInteractive& screen, ScreenState& state) {
    std::vector<std::string> frames = ReadFramesFromFile("message.txt");
    auto play_button = Button("Play Video", [&] {
      if (!frames.empty()) {
        PlayVideo(screen, frames, 4); // 4 frames per second
      }
    });
    auto back_button = Button("Back", [&] {
      state = ScreenState::MainMenu;
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
