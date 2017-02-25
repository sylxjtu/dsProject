all: ParserTest Interpreter

ParserTest: ASTNode.o Parser.o ParserTest.cpp

Interpreter: ASTNode.o Parser.o Interpreter.cpp

clean:
	-rm *.o && rm *.exe
