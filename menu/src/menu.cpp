#include <iostream>
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

enum class ScreenState {
  MainMenu,
  DocumentationMenu,
  FunctionMenu,
  Exit
};

void ShowMainMenu(ScreenInteractive& screen, ScreenState& state);
void ShowDocumentationMenu(ScreenInteractive& screen, ScreenState& state);
void ShowFunctionMenu(ScreenInteractive& screen, ScreenState& state, int function_selected);

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
      default:
        break;
    }
  }

  return 0;
}

void ShowMainMenu(ScreenInteractive& screen, ScreenState& state) {
  std::vector<std::string> entries = {"Glegisterex", "Gloginob", "Glocumentationeb", "GLEXITEL"};
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
  std::vector<std::string> syntax = {
    "gg register <username>", "gg login <options> <username>", "gg logout",
    "gg upload <options> <file>", "gg stream <options> <id>", "GLBACKEP"
  };
  std::vector<std::string> descriptions = {
    "Description for Glegisterex", "Description for Gloginob", "Description for Glogoutob",
    "Description for Gluploadem", "Description for Glestreamy"
  };

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

  auto function_name = Renderer([&] { return text("Function: " + functions[function_selected]) | bold | center; });

  auto function_syntax_title = text("Syntax: ");
  auto function_syntax_content = text(syntax[function_selected]) | inverted;
  auto function_syntax = Renderer([&] { return hbox({function_syntax_title, function_syntax_content}) | center; });
  auto function_description = Renderer([&] { return text("Description: " + descriptions[function_selected]) | center; });

  auto back_button = Button("Back", [&] {
    state = ScreenState::DocumentationMenu;
    screen.ExitLoopClosure()();
  });
  auto back_button_renderer = Renderer(back_button, [&] {
    return back_button->Render() | border | center;
  });

  Component table = Container::Vertical({
    function_name,
    function_syntax,
    function_description,
    back_button_renderer
  });

  screen.Loop(table);
}
