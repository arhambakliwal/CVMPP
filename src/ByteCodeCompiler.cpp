#include "../include/ByteCodeCompiler.h"

ByteCodeCompiler::ByteCodeCompiler():nextSlot(0), maxSlot(0)
{
      std::unordered_map<std::string, int> global;
      scopes.push_back(global);
}

std::vector<Instruction> ByteCodeCompiler::compile(const std::vector<Stmt*>& program)
{
      size_t i;
      for(i=0; i<program.size(); i++)
      {
            compileStmt(program[i]);
      }
      code.push_back(Instruction{Opcode::HALT});
      return code;
}

int ByteCodeCompiler::declareVar(const std::string& name)
{
      int index;
      index=nextSlot;
      scopes.back()[name]=index;
      nextSlot++;
      if(index>maxSlot)
      {
            maxSlot=index;
      }
      return index;
}

int ByteCodeCompiler::lookupVar(const std::string& name) const
{
      for(auto it=scopes.rbegin(); it!=scopes.rend(); it++)
      {
            if(it->count(name))
            {
                  return it->at(name);
            }
      }
      return -1;
}

void ByteCodeCompiler::compileExpr(Expr* expr)
{
      if(auto numexpr=dynamic_cast<NumberExpr*>(expr))
      {
            code.push_back(Instruction{Opcode::PUSH_INT, numexpr->val});
      }
      else if(dynamic_cast<InputExpr*>(expr))
      {
            code.push_back(Instruction{Opcode::INPUT});
      }
      else if(auto boolexpr=dynamic_cast<BoolExpr*>(expr))
      {
            code.push_back(Instruction{Opcode::PUSH_BOOL, boolexpr->val});
      }
      else if(auto varexpr=dynamic_cast<VarExpr*>(expr))
      {
            code.push_back(Instruction{Opcode::LOAD, lookupVar(varexpr->name)});
      }
      else if(auto binexpr=dynamic_cast<BinaryExpr*>(expr))
      {
            if(binexpr->op=="=")
            {
                  auto var=dynamic_cast<VarExpr*>(binexpr->left);
                  compileExpr(binexpr->right);
                  code.push_back(Instruction{Opcode::STORE, lookupVar(var->name)});
                  code.push_back(Instruction{Opcode::LOAD, lookupVar(var->name)});
            }
            else if(binexpr->op=="+")
            {
                  compileExpr(binexpr->left);
                  compileExpr(binexpr->right);
                  code.push_back(Instruction{Opcode::ADD});
            }
            else if(binexpr->op=="-")
            {
                  compileExpr(binexpr->left);
                  compileExpr(binexpr->right);
                  code.push_back({Opcode::SUB});
            }
            else if(binexpr->op=="*")
            {
                  compileExpr(binexpr->left);
                  compileExpr(binexpr->right);
                  code.push_back({Opcode::MUL});
            }
            else if(binexpr->op=="/")
            {
                  compileExpr(binexpr->left);
                  compileExpr(binexpr->right);
                  code.push_back({Opcode::DIV});
            }
            else if(binexpr->op=="%")
            {
                  compileExpr(binexpr->left);
                  compileExpr(binexpr->right);
                  code.push_back({Opcode::MOD});
            }
            else if(binexpr->op=="||")
            {
                  compileExpr(binexpr->left);
                  compileExpr(binexpr->right);
                  code.push_back(Instruction{Opcode::OR});      
            }
            else if(binexpr->op=="&&")
            {      
                  compileExpr(binexpr->left);
                  compileExpr(binexpr->right);
                  code.push_back(Instruction{Opcode::AND});
            }
            else if(binexpr->op=="==")
            {
                  compileExpr(binexpr->left);
                  compileExpr(binexpr->right);
                  code.push_back(Instruction{Opcode::EQ});
            }
            else if(binexpr->op=="!=")
            {
                  compileExpr(binexpr->left);
                  compileExpr(binexpr->right);
                  code.push_back(Instruction{Opcode::EQ});
                  code.push_back(Instruction{Opcode::NOT});
            }
            else if(binexpr->op=="<")
            {
                  compileExpr(binexpr->left);
                  compileExpr(binexpr->right);
                  code.push_back(Instruction{Opcode::LT});
            }
            else if(binexpr->op==">")
            {
                  compileExpr(binexpr->left);
                  compileExpr(binexpr->right);
                  code.push_back(Instruction{Opcode::GT});
            }
            else if(binexpr->op=="<=")
            {
                  compileExpr(binexpr->left);
                  compileExpr(binexpr->right);
                  code.push_back(Instruction{Opcode::GT});
                  code.push_back(Instruction{Opcode::NOT});
            }
            else if(binexpr->op==">=")
            {
                  compileExpr(binexpr->left);
                  compileExpr(binexpr->right);
                  code.push_back(Instruction{Opcode::LT});
                  code.push_back(Instruction{Opcode::NOT});
            }
      }
      else if(auto unexpr=dynamic_cast<UnaryExpr*>(expr))
      {
            if(unexpr->op=="!")
            {      
                  compileExpr(unexpr->operand);
                  code.push_back(Instruction{Opcode::NOT});
            }
            else if(unexpr->op=="-")
            {      
                  code.push_back(Instruction{Opcode::PUSH_INT, 0});
                  compileExpr(unexpr->operand);
                  code.push_back(Instruction{Opcode::SUB});
            }
            else if(unexpr->op=="+")
            {      
                  compileExpr(unexpr->operand);
            }
      }
            
}
void ByteCodeCompiler::compileStmt(Stmt* stmt)
{
      size_t i, jmp, jmp_false;
      if(auto decl=dynamic_cast<DeclarationStmt*>(stmt))
      {
            if(decl->initializer!=nullptr)
            {
                  compileExpr(decl->initializer);
            }
            else if(decl->type==DataType::INT)
            {
                  code.push_back(Instruction{Opcode::PUSH_INT, 0});
            }
            else if(decl->type==DataType::BOOL)
            {
                  code.push_back(Instruction{Opcode::PUSH_BOOL, 0});
            }
            code.push_back(Instruction{Opcode::STORE, declareVar(decl->name)});
      }
      else if(auto e=dynamic_cast<ExprStmt*>(stmt))
      {
            compileExpr(e->expr);
            code.push_back(Instruction{Opcode::POP});
      }
      else if(auto printstmt=dynamic_cast<PrintStmt*>(stmt))
      {
            if(printstmt->content!=nullptr)
            {
                  compileExpr(printstmt->content);
                  code.push_back(Instruction{Opcode::PRINT});
            }
            else
            {
                  code.push_back(Instruction{Opcode::PRINT, -1});
            }
      }
      else if(auto blk=dynamic_cast<BlockStmt*>(stmt))
      {
            std::unordered_map<std::string, int> block;
            int scopebeg=nextSlot;
            scopes.push_back(block);
            for(i=0; i<blk->block.size(); i++)
            {
                  compileStmt(blk->block[i]);
            }
            nextSlot=scopebeg;
            scopes.pop_back();
      }
      else if(auto ifstmt=dynamic_cast<IfStmt*>(stmt))
      {
            compileExpr(ifstmt->condition);
            code.push_back(Instruction{Opcode::JMP_IF_FALSE, -1});
            jmp_false=code.size()-1;
            compileStmt(ifstmt->If);
            if(ifstmt->Else!=nullptr)
            {
                  code.push_back(Instruction{Opcode::JMP, -1});
                  jmp=code.size()-1;
            }
            code[jmp_false].operand=code.size();
            if(ifstmt->Else!=nullptr)
            {
                  compileStmt(ifstmt->Else);
                  code[jmp].operand=code.size();
            }
      } 
      else if(auto whilestmt=dynamic_cast<WhileStmt*>(stmt))
      {
            int start, jmp_false;
            start=code.size();
            compileExpr(whilestmt->condition);
            code.push_back(Instruction{Opcode::JMP_IF_FALSE, -1});
            jmp_false=code.size()-1;
            compileStmt(whilestmt->body);
            code.push_back(Instruction{Opcode::JMP, start});
            code[jmp_false].operand=code.size();
      }
}

int ByteCodeCompiler::getMemorySize()
{
      return maxSlot+1;
}