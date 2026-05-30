#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <unordered_set>
#include "ErrorHandler.h"

enum class TokenType:int
{
      Identifier,
      Keyword,
      Number,
      Operator,
      SpecialChar
};

class Token
{
      public:
            TokenType type;
            std::string name;
            int line_no;
            Token()=default;
            Token(TokenType, std::string, int);
};
std::vector<Token> Tokenize(std::istream&);