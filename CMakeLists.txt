cmake_minimum_required(VERSION 3.14)
project(asdfasdf)

set(CMAKE_CXX_STANDARD 20)

add_executable(asdfasdf main.cpp main.cpp include/parser.hpp include/scanner.hpp  src/scanner.cpp src/fisrtSet.cpp src/followSet.cpp include/firstSet.hpp include/followSet.hpp)
