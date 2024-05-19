#include <iostream>
#include <sstream>
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

enum class ScreenState {
  MainMenu,
  DocumentationMenu,
  FunctionMenu,
  VideoScreen,
  Exit
};

void ShowMainMenu(ScreenInteractive& screen, ScreenState& state);
void ShowDocumentationMenu(ScreenInteractive& screen, ScreenState& state);
void ShowFunctionMenu(ScreenInteractive& screen, ScreenState& state, int function_selected);
void ShowVideoScreen(ScreenInteractive& screen, ScreenState& state);

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

  Component renderer = Renderer(menu, [&] {
    return vbox({
      text("Welcome to GLEEBLEGLOB!") | border | center | color(Color::CyanLight),
      menu->Render() | border,
    }) | center;
  });

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
    "Description for Glegisterex", "Description for Gloginob", "Description for Glogoutob",
    "Description for Gluploadem", "Description for Glestreamy"
  };

  auto function_name = Renderer([&] {
    return hbox({
      text("Function: ") | underlined,
      text(functions[function_selected])
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
    return hbox({function_description_title, function_description_content}) | center;
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
  std::string video_buffer = 
    "skdfnasdlcjiasdijofioasdjcpoasdjf\n"
    "skdfnasdlcjiasdijofioasdjcpoasdjf\n"
    "skdfnasdlcjiasdijofioasdjcpoasdjf\n"
    "skdfnasdlcjiasdijofioasdjcpoasdjf\n"
    "skdfnasdlcjiasdijofioasdjcpoasdjf\n"
    "skdfnasdlcjiasdijofioasdjcpoasdjf\n"
    "skdfnasdlcjiasdijofioasdjcpoasdjf";

  std::vector<Element> lines;
  std::istringstream stream(video_buffer);
  std::string line;
  while (std::getline(stream, line)) {
    lines.push_back(text(line));
  }

  auto video_content = Renderer([&] {
    return vbox({
      text("Video Buffer:") | underlined | center,
      vbox(std::move(lines)) | border | center
    });
  });

  auto back_button = Button("Back", [&] {
    state = ScreenState::MainMenu;
    screen.ExitLoopClosure()();
  });
  auto back_button_renderer = Renderer(back_button, [&] {
    return back_button->Render() | center;
  });

  Component content = Container::Vertical({
    video_content,
    back_button_renderer
  });

  screen.Loop(content);
}
