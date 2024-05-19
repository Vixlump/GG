# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-src"
  "/Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-build"
  "/Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/ftxui-populate-prefix"
  "/Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/ftxui-populate-prefix/tmp"
  "/Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp"
  "/Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src"
  "/Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/saaranganand/Desktop/stormhack/GG/menu/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
