#include <iostream>
#include <sstream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>

#include "ftxui/component/component.hpp"
// imported in gg.hpp: #include "ftxui/component/screen_interactive.hpp"

#include "gg.hpp"

using namespace ftxui;

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
      default:
        break;
    }
  }

  return 0;
}
