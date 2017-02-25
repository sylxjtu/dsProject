CXXFLAGS = -std=c++11 -g -O3 -march=native
export CXXFLAGS

.PHONY: all Interpreter Maze doc clean

all: Interpreter Maze doc

Interpreter:
	$(MAKE) -C src/Interpreter Interpreter

Maze:
	$(MAKE) -C src/Maze Maze

doc:
	$(MAKE) -C doc

clean:
	$(MAKE) -C src/Interpreter clean
	$(MAKE) -C src/Maze clean
	$(MAKE) -C doc clean
