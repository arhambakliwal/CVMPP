#pragma once
#include <string>
#include <cstdlib>
#include <iostream>
#include <vector>
class ErrorHandler
{
      private:
            static std::vector<std::string> program;
      public:
            static void readProgram(std::istream&);
            static void lexicalError(int, const std::string&);
            static void syntaxError(int, const std::string&);
            static void semanticError(int, const std::string&);
            static void runtimeError(const std::string&);
};