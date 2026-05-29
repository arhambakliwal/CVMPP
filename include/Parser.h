#pragma once
#include "AST.h"
#include "ErrorHandler.h"
#include <map>
#include <algorithm>
#include <unordered_map>

class Parser
{
      private:
            std::unordered_map<std::string, int> intVars;
            std::unordered_map<std::string, bool> boolVars;
            std::unordered_map<std::string, double> left_bind;
            std::unordered_map<std::string, double> right_bind;
            Expr* ParseExpr(double);
            std::vector<Token>& tokens;
      public:
            Parser(std::vector<Token>&);
            std::vector<Stmt*> ParseProgram();
            Stmt* ParseBlock();
            Stmt* ParseStmt();
            Stmt* ParseDeclaration();
            Stmt* ParseIf();
            Stmt* ParseWhile();
            Stmt* ParsePrint();
            Expr* ParseExpr();
};
