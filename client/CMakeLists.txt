cmake_minimum_required(VERSION 3.27.1)
project(gg VERSION 0.1 DESCRIPTION "Command-Line Interface for GleebleGlob Service" LANGUAGES C CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)

set(CMAKE_CXX_FLAGS "-std=c++20 ${CMAKE_CXX_FLAGS}")

add_executable(${PROJECT_NAME}
	gg_client.cpp
	main.cpp
)

target_link_libraries(${PROJECT_NAME} PkgConfig::SDL2)
