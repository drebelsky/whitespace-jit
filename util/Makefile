default: main

CXXFLAGS=-O1 -Wall -Wpedantic -Werror

main: main.cc
	g++ -o $@ main.cc ../jit.o $(CXXFLAGS)

.PHONY: default main
