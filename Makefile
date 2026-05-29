all: compiler

compiler: build/main.o  build/ErrorHandler.o build/Lexer.o build/AST.o build/Parser.o build/SemanticAnalyzer.o build/Instruction.o build/ByteCodeCompiler.o build/VirtualMachine.o
	g++ -Wall -Wextra -std=c++17 build/ErrorHandler.o build/Lexer.o build/AST.o build/Parser.o build/SemanticAnalyzer.o build/Instruction.o build/ByteCodeCompiler.o build/VirtualMachine.o build/main.o -o compiler

build:
	mkdir -p build

build/ErrorHandler.o: src/ErrorHandler.cpp include/ErrorHandler.h | build
	g++ -c -Wall -Wextra -std=c++17 src/ErrorHandler.cpp -o build/ErrorHandler.o
 
build/Lexer.o: src/Lexer.cpp include/Lexer.h | build
	g++ -c -Wall -Wextra -std=c++17 src/Lexer.cpp -o build/Lexer.o

build/AST.o: src/AST.cpp include/AST.h | build
	g++ -c -Wall -Wextra -std=c++17 src/AST.cpp -o build/AST.o

build/Parser.o: src/Parser.cpp include/Parser.h | build
	g++ -c -Wall -Wextra -std=c++17 src/Parser.cpp -o build/Parser.o

build/SemanticAnalyzer.o: src/SemanticAnalyzer.cpp include/SemanticAnalyzer.h | build
	g++ -c -Wall -Wextra -std=c++17 src/SemanticAnalyzer.cpp -o build/SemanticAnalyzer.o

build/ByteCodeCompiler.o: src/ByteCodeCompiler.cpp include/ByteCodeCompiler.h | build
	g++ -c -Wall -Wextra -std=c++17 src/ByteCodeCompiler.cpp -o build/ByteCodeCompiler.o

build/Instruction.o: src/Instruction.cpp include/Instruction.h | build
	g++ -c -Wall -Wextra -std=c++17 src/Instruction.cpp -o build/Instruction.o

build/VirtualMachine.o: src/VirtualMachine.cpp include/VirtualMachine.h | build
	g++ -c -Wall -Wextra -std=c++17 src/VirtualMachine.cpp -o build/VirtualMachine.o

build/main.o: src/main.cpp | build
	g++ -c -Wall -Wextra -std=c++17 src/main.cpp -o build/main.o
clean:
	rm -rf build compiler