#pragma once
#include "Instruction.h"
#include "AST.h"
#include <unordered_map>

class ByteCodeCompiler
{
      private:
            int nextSlot;
            std::vector<Instruction> code;
            std::vector<std::unordered_map<std::string, int>> scopes;
            void compileStmt(Stmt*);
            void compileExpr(Expr*);
            int declareVar(const std::string& name);
            int lookupVar(const std::string& name) const;
            int maxSlot;
      public:
            int getMemorySize();
            ByteCodeCompiler();
            std::vector<Instruction> compile(const std::vector<Stmt*>&);
};