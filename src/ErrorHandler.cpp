#include "../include/ErrorHandler.h"
std::vector<std::string> ErrorHandler::program;
void ErrorHandler::readProgram(std::istream& prog)
{
      std::string line;
      while(std::getline(prog, line))
      {
            program.push_back(line);
      }
}

void ErrorHandler::lexicalError(int line_no, const std::string& error_msg)
{
      std::cerr<<"Lexical Error (Line "<<line_no<<"):\n\t"<<program.at(line_no-1)<<"\n\t"<<error_msg<<std::endl;
      std::exit(1);
}

void ErrorHandler::syntaxError(int line_no , const std::string& error_msg)
{
      std::cerr<<"Syntax Error (Line "<<line_no<<"):\n\t"<<program.at(line_no-1)<<"\n\t"<<error_msg<<std::endl;
      std::exit(1);
}

void ErrorHandler::semanticError(int line_no, const std::string& error_msg)
{
      std::cerr<<"Semantic Error (Line "<<line_no<<"):\n\t"<<program.at(line_no-1)<<"\n\t"<<error_msg<<std::endl;
      std::exit(1);
}

void ErrorHandler::runtimeError(const std::string& error_msg)
{
      std::cerr<<"Runtime Error:"<<"\n\t"<<error_msg<<std::endl;
      std::exit(1);
}