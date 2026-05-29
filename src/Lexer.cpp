#include "../include/Lexer.h"

Token::Token(TokenType type, std::string name, int line_no):type(type), name(name), line_no(line_no)
{
}

std::vector<Token> Tokenize(std::istream& is)
{
      size_t i, len, best_l;
      int line_no;
      std::string id, test;
      std::vector<Token> tokens;
      std::string line;
      std::string temp;
      TokenType t;
      std::unordered_set<std::string> keywords={"if", "else", "while", "print", "input", "int", "bool", "true", "false"};
      std::unordered_set<std::string> operators={"+", "-", "*", "/", "%", "||", "&&", "!", "==", "!=", "<", ">", "<=", ">=", "="};
      std::unordered_set<std::string> specialChars={"(", ")", "{", "}", ";", "//"};
      for(line_no=1; std::getline(is, line); line_no++)
      {
            i=0;
            while(i<line.size())
            {
                  //identifier or keyword
                  if(isalpha(line[i])||line[i]=='_')
                  {
                        for(; i<line.size() && (isalnum(line[i])||line[i]=='_'); i++)
                        {
                              id.push_back(line[i]);
                        }
                        if(keywords.count(id)==0)
                        {
                              tokens.push_back(Token{TokenType::Identifier, id, line_no});
                        }
                        else
                        {
                              tokens.push_back(Token{TokenType::Keyword, id, line_no});
                        }
                        id.clear();
                  }
                  //number
                  else if(isdigit(line[i]))
                  {
                        for(; i<line.size() && isdigit(line[i]); i++)
                        {
                              id.push_back(line[i]);
                        }
                        tokens.push_back(Token{TokenType::Number, id, line_no});
                        id.clear();
                  }
                  //whitespace
                  else if(isspace(line[i]))
                  {
                        i++;
                  }
                  //others
                  else
                  {
                        test.clear();
                        for(len=1, best_l=0; i+len<=line.size(); len++)
                        {
                              test.push_back(line[i+len-1]);
                              if(operators.count(test))
                              {
                                    //operator
                                    id=test;
                                    best_l=id.size();
                                    t=TokenType::Operator;
                              }
                              else if(specialChars.count(test))
                              {
                                    //special character
                                    id=test;
                                    best_l=id.size();
                                    t=TokenType::SpecialChar;
                                    if(id=="//")
                                    {
                                          break;
                                    }
                              }
                        }
                        if(id=="//")//single line comments
                        {
                              id.clear();
                              break;
                        }
                        else if(best_l>0)
                        {
                              tokens.push_back(Token{t, id, line_no});
                        }
                        //invalid token
                        else
                        {
                              temp.push_back(line[i]);
                              ErrorHandler::lexicalError(line_no, "Unexpected character \'"+temp+"\'");
                        }
                        id.clear();
                        i+=best_l;
                  }
            }
      }
      return tokens;
}