#include "../include/Lexer.h"
#include "../include/AST.h"
#include "../include/Parser.h"
#include "../include/SemanticAnalyzer.h"
#include "../include/ByteCodeCompiler.h"
#include "../include/VirtualMachine.h"
#include <fstream>
#include <algorithm>

int main(int argc, char* argv[])
{
      size_t i;
      bool debug;
      std::vector<Token> tokens;
      std::ifstream prog;
      std::ifstream fin;
      std::vector<Stmt*> program;
      std::vector<Instruction> bytecode;
      if(argc<=1)
      {
            std::cerr<<"Fatal Error: no input file\n";
            std::exit(1);
      }
      prog.open(argv[1]);
      ErrorHandler::readProgram(prog);
      fin.open(argv[1]);
      if(!fin.is_open())
      {
            std::cerr<<"Fatal Error: "+std::string(argv[1])+":no such file\n";
            std::exit(1);
      }
      debug=false;
      if(argc>2)
      {
            if(std::string(argv[2])=="1")
            {
                  debug=true;
            }
      }
      // Lexical Analysis
      tokens=Tokenize(fin);
      // Parsing
      reverse(tokens.begin(), tokens.end());
      Parser p(tokens);
      program=p.ParseProgram();
      if(debug)
      {
            std::cout<<"AST\n";
            for(i=0; i<program.size(); i++)
            {
                  program[i]->print();
            }
            std::cout<<"----------------------------------------\n";
      }
      // Semantic Analysis
      SemanticAnalyzer analyzer;
      analyzer.analyze(program);
      // Byte Code Compilation
      ByteCodeCompiler bcc;
      bytecode=bcc.compile(program);
      if(debug)
      {
            std::cout<<"Byte Code\n";
            for(i=0; i<bytecode.size(); i++)
            {
                  std::cout<<i<<". "<<bytecode[i]<<std::endl;
            }
            std::cout<<"----------------------------------------\n";
      }

      // Execution
      VirtualMachine cvm(bytecode, bcc.getMemorySize());
      cvm.run();
      //clean up
      for(i=0; i<program.size(); i++)
      {
            delete program[i];
      }
      return 0;
}