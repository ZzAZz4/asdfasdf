main = main.cpp
cppFiles = src/common.cpp src/parser.cpp src/scanner.cpp

all: compile execute


compile:
    g++ -std=c++17 $(main) 

execute:
    ./a.out
