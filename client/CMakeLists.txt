cmake_minimum_required(VERSION 3.27.1)
project(gg VERSION 0.1 DESCRIPTION "Command-Line Interface for GleebleGlob Service" LANGUAGES C CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG main # Important: Specify a version or a commit hash here.
)
FetchContent_MakeAvailable(ftxui)

set(CMAKE_CXX_FLAGS "-std=c++20 ${CMAKE_CXX_FLAGS}")

add_executable(${PROJECT_NAME}
	gg.cpp
	gg_client.cpp
	main.cpp
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE PkgConfig::SDL2
	PRIVATE ftxui::component
)
