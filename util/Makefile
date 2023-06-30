default: main

CXXFLAGS=-O1 -Wall -Wpedantic -Werror -std=c++17

main: main.cc
	g++ -o $@ main.cc ../jit.cc $(CXXFLAGS)

.PHONY: default main
