# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild

# Utility rule file for ftxui-populate.

# Include any custom commands dependencies for this target.
include CMakeFiles/ftxui-populate.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ftxui-populate.dir/progress.make

CMakeFiles/ftxui-populate: CMakeFiles/ftxui-populate-complete

CMakeFiles/ftxui-populate-complete: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-install
CMakeFiles/ftxui-populate-complete: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-mkdir
CMakeFiles/ftxui-populate-complete: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-download
CMakeFiles/ftxui-populate-complete: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-update_disconnected
CMakeFiles/ftxui-populate-complete: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-patch_disconnected
CMakeFiles/ftxui-populate-complete: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-configure
CMakeFiles/ftxui-populate-complete: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-build
CMakeFiles/ftxui-populate-complete: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-install
CMakeFiles/ftxui-populate-complete: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-test
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'ftxui-populate'"
	/opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E make_directory /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/CMakeFiles
	/opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E touch /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/CMakeFiles/ftxui-populate-complete
	/opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E touch /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-done

ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-build: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "No build step for 'ftxui-populate'"
	cd /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-build && /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E echo_append
	cd /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-build && /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E touch /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-build

ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-configure: ftxui-populate-prefix/tmp/ftxui-populate-cfgcmd.txt
ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-configure: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-patch_disconnected
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "No configure step for 'ftxui-populate'"
	cd /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-build && /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E echo_append
	cd /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-build && /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E touch /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-configure

ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-download: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-gitinfo.txt
ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-download: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (git clone) for 'ftxui-populate'"
	cd /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps && /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -P /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/ftxui-populate-prefix/tmp/ftxui-populate-gitclone.cmake
	cd /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps && /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E touch /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-download

ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-install: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-build
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No install step for 'ftxui-populate'"
	cd /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-build && /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E echo_append
	cd /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-build && /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E touch /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-install

ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Creating directories for 'ftxui-populate'"
	/opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -Dcfgdir= -P /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/ftxui-populate-prefix/tmp/ftxui-populate-mkdirs.cmake
	/opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E touch /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-mkdir

ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-patch_disconnected: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-patch-info.txt
ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-patch_disconnected: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-update_disconnected
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No patch_disconnected step for 'ftxui-populate'"
	/opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E echo_append
	/opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E touch /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-patch_disconnected

ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-test: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-install
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "No test step for 'ftxui-populate'"
	cd /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-build && /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E echo_append
	cd /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-build && /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E touch /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-test

ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-update_disconnected: ftxui-populate-prefix/tmp/ftxui-populate-gitupdate.cmake
ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-update_disconnected: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-update-info.txt
ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-update_disconnected: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-download
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Performing disconnected update step for 'ftxui-populate'"
	cd /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-src && /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -Dcan_fetch=NO -P /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/ftxui-populate-prefix/tmp/ftxui-populate-gitupdate.cmake
	cd /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-src && /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E touch /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-update_disconnected

ftxui-populate: CMakeFiles/ftxui-populate
ftxui-populate: CMakeFiles/ftxui-populate-complete
ftxui-populate: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-build
ftxui-populate: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-configure
ftxui-populate: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-download
ftxui-populate: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-install
ftxui-populate: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-mkdir
ftxui-populate: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-patch_disconnected
ftxui-populate: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-test
ftxui-populate: ftxui-populate-prefix/src/ftxui-populate-stamp/ftxui-populate-update_disconnected
ftxui-populate: CMakeFiles/ftxui-populate.dir/build.make
.PHONY : ftxui-populate

# Rule to build all files generated by this target.
CMakeFiles/ftxui-populate.dir/build: ftxui-populate
.PHONY : CMakeFiles/ftxui-populate.dir/build

CMakeFiles/ftxui-populate.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ftxui-populate.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ftxui-populate.dir/clean

CMakeFiles/ftxui-populate.dir/depend:
	cd /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild /Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/CMakeFiles/ftxui-populate.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/ftxui-populate.dir/depend

