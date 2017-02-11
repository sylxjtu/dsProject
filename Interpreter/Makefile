CXXFLAGS = -std=c++11 -g -O3 -march=native
all: ParserTest Interpreter
ParserTest: ASTNode.o Parser.o
Interpreter: ASTNode.o Parser.o
clean:
	rm *.o && rm *.exe