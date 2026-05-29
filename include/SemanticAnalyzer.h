#pragma once
#include "AST.h"
#include <unordered_map>
class SemanticAnalyzer
{
      private:
            std::unordered_map<std::string, std::unordered_set<DataType>> operatorType;
            std::unordered_map<std::string, DataType> operatorResultType;
            std::vector<std::unordered_map<std::string, DataType>> scopes;
            DataType evaluate(Expr*);
            void handleStmt(Stmt*);
            void handleBlock(BlockStmt*);
            void handleDeclaration(DeclarationStmt*);
            void handleIf(IfStmt*);
            void handleWhile(WhileStmt*);
            void handlePrint(PrintStmt*);
      public:
            SemanticAnalyzer();
            void analyze(std::vector<Stmt*>&);
};