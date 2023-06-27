default: compile

CXXFLAGS  = -O3 -Wall -Wpedantic -Werror -std=c++17
CXX      ?= g++
CC_FILES  = $(wildcard *.cc)
H_FILES   = $(wildcard *.h)
OBJ_FILES = $(patsubst %.cc, %.o, $(CC_FILES))

debug: CXXFLAGS=-O0 -g -Wall -Wpedantic -Werror -std=c++17 -fsanitize=address

debug: $(CC_FILES)
	$(CXX) -o $@ $(CC_FILES) $(CXXFLAGS)

clean:
	rm -f *.o debug compile

format:
	clang-format -i $(CC_FILES) $(H_FILES)

%.o: %.cc %.h datatypes.h
	$(CXX) -c $< $(CXXFLAGS)

compile: $(OBJ_FILES)
	$(CXX) -o $@ $^ $(CXXFLAGS)

# Note jit_size.h can't depend on jit.o because jit.o requires jit_size.h
# Also, jit.o can't require jit_size.h because of the circular dependencies
jit_size.h:
	cd util && $(MAKE) && cd ..
	./util/main > jit_size.h

.PHONY: default clean jit_size.h format
