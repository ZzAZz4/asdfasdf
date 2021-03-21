main = test.cpp
cppFiles = src/common.cpp src/fisrtSet.cpp src/followSet.cpp

all: compile execute


compile:
	g++ -std=c++17 $(main) 

execute:
	./a.out