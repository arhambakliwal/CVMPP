#include "../include/Parser.h"
Parser::Parser(std::vector<Token>& tokens):tokens(tokens)
{
      //left binding values      
      left_bind={
            {"+", 3.0}, {"-", 3.0},
            {"/", 3.5}, {"*", 3.5}, {"%", 3.5},
            {"==", 1.2}, {"!=", 1.2},
            {"<", 1.5}, {">", 1.5}, {"<=", 1.5}, {">=", 1.5},
            {"||", 0.5}, {"&&", 0.8},
            {"=", 0.2}
      };
      
      //right binding values
      right_bind={
            {"#+", 4.0}, {"#-", 4.0}, {"#!", 4.0},
            {"+", 3.1}, {"-", 3.1},
            {"/", 3.6}, {"*", 3.6}, {"%", 3.6},
            {"==", 1.3}, {"!=", 1.3},
            {"<", 1.6}, {">", 1.6}, {"<=", 1.6}, {">=", 1.6},
            {"||", 0.6}, {"&&", 0.9},
            {"=", 0.1}
      };
      // higher the binding value, higher the preceedence
      // NOTE: A unary operator has a prefix # here
      // Eg. #- is unary -
};
Expr* Parser::ParseExpr()
{
      return ParseExpr(0);
}
Expr* Parser::ParseExpr(double min_bp)
{
      Expr* lhs=nullptr;
      Expr* rhs=nullptr;
      Token t, op;
      int line_no;
      if(tokens.size()>0)
      {
            t=tokens.back();
            tokens.pop_back();
            line_no=t.line_no;
      }
      else
      {
            return nullptr;
      }
      if(t.type==TokenType::Number)
      {
            lhs=new NumberExpr(line_no, std::stoi(t.name));
      }
      else if(t.name=="true")
      {
            lhs=new BoolExpr(line_no, true);      
      }
      else if(t.name=="false")
      {
            lhs=new BoolExpr(line_no, false);
      }
      else if(t.name=="input")
      {
            if(tokens.size()>0)
            {
                  if(tokens.back().name=="(")
                  {
                        tokens.pop_back();
                        if(tokens.size()>0)
                        {
                              if(tokens.back().name==")")
                              {
                                    tokens.pop_back();
                              }
                              else
                              {
                                    ErrorHandler::syntaxError(line_no, "Expected  \')\' before \'"+tokens.back().name+"\' token");
                              }
                        }
                        else
                        {
                              ErrorHandler::syntaxError(line_no, "Expected  \')\' before the end of input");
                        }
                  }
                  else
                  {
                        ErrorHandler::syntaxError(line_no, "Expected  \'(\' before \'"+tokens.back().name+"\' token");
                  }
            }
            else
            {
                  ErrorHandler::syntaxError(line_no, "Expected  \'(\' before the end of input");
            }
            lhs=new InputExpr(line_no);
      }
      else if(t.type==TokenType::Identifier)
      {
            lhs=new VarExpr(line_no, t.name);
      }
      else if(t.name=="(")
      {
            lhs=ParseExpr(0);
            if(tokens.size()==0)
            {
                  //error
                  ErrorHandler::syntaxError(line_no, "Expected  /')/' before the end of input");
            }
            else if(tokens.back().name!=")")
            {
                  //error
                  ErrorHandler::syntaxError(line_no, "Expected  /')/' before \'"+tokens.back().name+"\' token");
            }
            else
            {
                  tokens.pop_back();
            }
      }
      else if(t.type==TokenType::Operator && right_bind.count("#"+t.name))
      {
            rhs=ParseExpr(right_bind.at("#"+t.name));
            lhs=new UnaryExpr(line_no, t.name, rhs);
      }
      else
      {
            // error
            ErrorHandler::syntaxError(line_no, "Expected primary-expression before \'"+t.name+"\'");
      }
      while(tokens.size()>0)
      {
            op=tokens.back();
            if(op.name==")" || op.name==";" || op.name=="{" || op.name=="}")
            {
                  break;
            }
            else if(op.type==TokenType::Keyword&&op.name!="input"&&op.name!="true"&&op.name!="false")
            {
                  break;
            }
            if(left_bind.count(op.name)==0)
            {
                  //error
                  ErrorHandler::syntaxError(line_no, "Expected primary-expression before \'"+op.name+"\'");
            }
            else if(left_bind.at(op.name)<min_bp)
            {
                  break;
            }
            tokens.pop_back();
            rhs=ParseExpr(right_bind.at(op.name));
            if(rhs==nullptr)
            {
                  //error
                  if(tokens.size()>0)
                  {
                        ErrorHandler::syntaxError(line_no, "Expected primary-expression before \'"+tokens.back().name+"\'");
                  }
                  else
                  {
                        ErrorHandler::syntaxError(line_no, "Expected primary-expression before the end of input");
                  }
            }
            lhs=new BinaryExpr(line_no, op.name, lhs, rhs);
      }
      return lhs;
}

std::vector<Stmt*> Parser::ParseProgram()
{
      std::vector<Stmt*> program;
      while(tokens.size()>0)
      {
            program.push_back(ParseStmt());
      }
      return program;
}

Stmt* Parser::ParseBlock()
{
      int line_no;
      std::vector<Stmt*> stmts;
      line_no=tokens.back().line_no;
      tokens.pop_back();
      while(tokens.size()>0)
      {
            if(tokens.back().name=="}")
            {
                  tokens.pop_back();
                  return new BlockStmt(line_no, stmts);
            }
            else
            {
                  line_no=tokens.back().line_no;
                  stmts.push_back(ParseStmt());
            }
      }
      //error
      ErrorHandler::syntaxError(line_no, "Expected \'}\' before the end of input");
      return nullptr;
}

Stmt* Parser::ParseWhile()
{
      int line_no;
      Expr* condition;
      Stmt* body=nullptr;
      line_no=tokens.back().line_no;
      tokens.pop_back();
      if(tokens.size()>0)
      {
            if(tokens.back().name!="(")
            {
                  ErrorHandler::syntaxError(tokens.back().line_no, "Expected '(' before \'"+tokens.back().name+"\'");
            }
            condition=ParseExpr();
            if(condition==nullptr)
            {
                  //error
                  if(tokens.size()>0)
                  {
                        ErrorHandler::syntaxError(tokens.back().line_no, "Expected an expression before \'"+tokens.back().name+"\'");
                  }
                  else
                  {
                        ErrorHandler::syntaxError(line_no, "Expected an expression before the end of input");
                  }
            }
            body=ParseStmt();
      }
      else
      {
            ErrorHandler::syntaxError(line_no, "Expected '(' before the end of input");
      }
      return new WhileStmt(line_no, condition, body);
}

Stmt* Parser::ParseStmt()
{
      int line_no;
      Token t;
      Expr* expr;
      Stmt* statement=nullptr;
      if(tokens.size()>0)
      {
            t=tokens.back();
            line_no=tokens.back().line_no;
            if(t.type==TokenType::Keyword &&t.name!="input"&&t.name!="true"&&t.name!="false")
            {
                  if(t.name=="if")
                  {
                        statement=ParseIf();
                  }
                  else if(t.name=="else")
                  {
                        ErrorHandler::syntaxError(t.line_no, "\'else\' without a previous \'if\'");
                  }
                  else if(t.name=="int" || t.name=="bool")
                  {
                        statement=ParseDeclaration();
                        
                  }
                  else if(t.name=="print")
                  {
                        statement=ParsePrint();
                        
                  }
                  else if(t.name=="while")
                  {
                        statement=ParseWhile();
                        
                  }
            }
            else if(t.type==TokenType::Identifier||t.name=="input"||t.name=="true"||t.name=="false")
            {
                  line_no=t.line_no;
                  expr=ParseExpr();
                  if(tokens.size()>0 && tokens.back().name==";")
                  {
                        tokens.pop_back();
                        statement=new ExprStmt(line_no, expr);
                  }
                  else if(tokens.size()>0)
                  {
                        //error
                        ErrorHandler::syntaxError(tokens.back().line_no, "Expected \';\' before \'"+tokens.back().name+"\'");
                  }
                  else
                  {
                        //error
                        ErrorHandler::syntaxError(line_no, "Expected \';\' before the end of input");
                  }
                  
            }
            else if(t.type==TokenType::SpecialChar)
            {
                  if(t.name=="{")
                  {
                        statement=ParseBlock();
                  }
            }
            else
            {
                  //error
                  ErrorHandler::syntaxError(line_no, "unexpected token \'"+t.name+"\'");
            }
      }
      return statement;
}

Stmt* Parser::ParseDeclaration()
{
      int line_no;
      std::string type, name;
      Expr* val=nullptr;
      type=tokens.back().name;
      line_no=tokens.back().line_no;
      tokens.pop_back();
      if(tokens.size()>0)
      {
            if(tokens.back().type==TokenType::Identifier)
            {
                  name=tokens.back().name;
                  tokens.pop_back();
                  if(tokens.size()>0)
                  {
                        if(tokens.back().name=="=")
                        {
                              tokens.pop_back();
                              val=ParseExpr();
                        }
                        if(tokens.size()>0 && tokens.back().name==";")
                        {
                              tokens.pop_back();
                        }
                        else if(tokens.size()>0)
                        {
                              //error
                              ErrorHandler::syntaxError(tokens.back().line_no, "Expected \';\' before \'"+tokens.back().name+"\'");
                        }
                        else
                        {
                              //error
                              ErrorHandler::syntaxError(line_no, "Expected \';\' before the end of input");
                        }
                  }
                  else
                  {
                        //error
                        ErrorHandler::syntaxError(line_no, "Expected \';\' before the end of input");
                  }
            }
            else
            {
                  //error
                  ErrorHandler::syntaxError(tokens.back().line_no, "Expected an identifier before \'"+tokens.back().name+"\'");
            }
            if(type=="int")
            {
                  return new DeclarationStmt(line_no, DataType::INT, name, val);
            }
            else if(type=="bool")
            {
                  return new DeclarationStmt(line_no, DataType::BOOL, name, val);
            }
            else
            {
                  ErrorHandler::syntaxError(line_no, "Unknown data type: \'"+type+"\'");
                  return nullptr;
            }
      }
      else
      {
            //error
            ErrorHandler::syntaxError(line_no, "Expected an identifier before the end of input");
            return nullptr;
      }
}

Stmt* Parser::ParseIf()
{
      int line_no;
      Expr* condition;
      Stmt* ifPart=nullptr;
      Stmt* elsePart=nullptr;
      line_no=tokens.back().line_no;
      tokens.pop_back();
      if(tokens.size()>0)
      {
            if(tokens.back().name!="(")
            {
                  ErrorHandler::syntaxError(tokens.back().line_no, "Expected '(' before \'"+tokens.back().name+"\'");
            }
            condition=ParseExpr();
            if(condition==nullptr)
            {
                  //error
                  if(tokens.size()>0)
                  {
                        ErrorHandler::syntaxError(tokens.back().line_no, "Expected an expression before \'"+tokens.back().name+"\'");
                  }
                  else
                  {
                        ErrorHandler::syntaxError(line_no, "Expected an expression before the end of input");
                  }
            }
            ifPart=ParseStmt();
      }
      else
      {
            //error
            ErrorHandler::syntaxError(line_no, "Expected '(' before the end of input");
      }
      if(tokens.size()>0)
      {
            if(tokens.back().name=="else")
            {
                  tokens.pop_back();
                  elsePart=ParseStmt();
            }
      }
      return new IfStmt(line_no, condition, ifPart, elsePart);
}

Stmt* Parser::ParsePrint()
{
      int line_no;
      Expr* content;
      line_no=tokens.back().line_no;
      tokens.pop_back();
      if(tokens.size()>0)
      {
            if(tokens.back().name!="(")
            {
                  //error
                  ErrorHandler::syntaxError(tokens.back().line_no, "Expected '(' before \'"+tokens.back().name+"\'");
            }
      }
      else
      {
            //error
            ErrorHandler::syntaxError(line_no, "Expected '(' before the end of input");
      }
      content=ParseExpr();
      if(tokens.size()>0 && tokens.back().name==";")
      {
            tokens.pop_back();
            return new PrintStmt(line_no, content);
      }
      else if(tokens.size()>0)
      {
            //error
            ErrorHandler::syntaxError(tokens.back().line_no, "Expected \';\' before \'"+tokens.back().name+"\'");
            return nullptr;
      }
      else
      {
            //error
            ErrorHandler::syntaxError(line_no, "Expected \';\' before the end of input");
            return nullptr;
      }
}