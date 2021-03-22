cmake_minimum_required(VERSION 3.14)
project(asdfasdf)

set(CMAKE_CXX_STANDARD 20)

add_executable(asdfasdf main.cpp include/common.hpp include/firstSet.hpp include/followSet.hpp include/parser.hpp include/scanner.hpp src/common.cpp src/fisrtSet.cpp src/followSet.cpp  src/scanner.cpp
        json/json.hpp json/json.hpp)
