cmake_minimum_required(VERSION 3.14)
project(asdfasdf)

set(CMAKE_CXX_STANDARD 17)

file(GLOB asdfasdf_SRC
        "include/*.h"
        "src/*.cpp")

add_executable(asdfasdf main.cpp ${asdfasdf_SRC})
