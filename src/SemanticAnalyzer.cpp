#include "../include/SemanticAnalyzer.h"

SemanticAnalyzer::SemanticAnalyzer()
{
      operatorType={
            {"+",{DataType::INT}}, {"-",{DataType::INT}}, {"*",{DataType::INT}}, {"/",{DataType::INT}}, {"%",{DataType::INT}},
            {"&&",{DataType::BOOL}}, {"||",{DataType::BOOL}}, {"!",{DataType::BOOL}},
            {"==",{DataType::INT, DataType::BOOL}}, {"!=",{DataType::INT, DataType::BOOL}},
            {"<",{DataType::INT}}, {">",{DataType::INT}}, {"<=",{DataType::INT}}, {">=",{DataType::INT}},
            {"=",{DataType::INT, DataType::BOOL}} 
      };
      
      operatorResultType={
            {"+",DataType::INT}, {"-",DataType::INT}, {"*",DataType::INT}, {"/",DataType::INT}, {"%",DataType::INT},
            {"&&",DataType::BOOL}, {"||",DataType::BOOL}, {"!",DataType::BOOL},
            {"==",DataType::BOOL}, {"!=",DataType::BOOL},
            {"<",DataType::BOOL}, {">",DataType::BOOL}, {"<=",DataType::BOOL}, {">=",DataType::BOOL},
            {"=",DataType::UNKNOWN} 
      };
      std::unordered_map<std::string, DataType> global;
      scopes.push_back(global);
}

DataType SemanticAnalyzer::evaluate(Expr* expr)
{
      bool found;
      DataType left_type;
      DataType right_type;
      DataType operand_type;
      if(dynamic_cast<NumberExpr*>(expr))
      {
            expr->type=DataType::INT;
      }
      else if(dynamic_cast<BoolExpr*>(expr))
      {
            expr->type=DataType::BOOL;
      }
      else if(dynamic_cast<InputExpr*>(expr))
      {
            expr->type=DataType::INT;
      }
      else if(auto e=dynamic_cast<VarExpr*>(expr))
      {
            found=false;
            for(auto it=scopes.rbegin(); it!=scopes.rend(); it++)
            {
                  auto it1=it->find(e->name);
                  if(it1!=it->end())
                  {
                        expr->type=it1->second;
                        found=true;
                        break;
                  }
            }
            if(!found)
            {
                  //error variable undeclared
                  ErrorHandler::semanticError(expr->line_no,"\'"+e->name+"\' was not declared in this scope");
            }
      }
      else if(auto e=dynamic_cast<BinaryExpr*>(expr))
      {
            if(e->type==DataType::UNKNOWN)
            {
                  left_type=evaluate(e->left);
                  right_type=evaluate(e->right);
                  if(e->op=="=")
                  {
                        if(dynamic_cast<VarExpr*>(e->left))
                        {
                              if(left_type==right_type)
                              {
                                    //good
                                    expr->type=right_type;
                              }
                              else
                              {
                                    //error type mismatch
                                    ErrorHandler::semanticError(expr->line_no,"type mismatch between the operands of \'"+e->op+"\'");
                              }
                        }
                        else
                        {
                              //error assigning to an rvalue
                              ErrorHandler::semanticError(expr->line_no,"lvalue required as left operand of assignment");
                        }
                  }
                  else if(left_type==right_type)
                  {
                        if(operatorType.at(e->op).count(right_type))
                        {
                              expr->type=operatorResultType.at(e->op);
                        }
                        else
                        {
                              //error unsupported operand type
                              ErrorHandler::semanticError(expr->line_no,"unsupported operand types for operator \'"+e->op+"\'");
                        }
                  }
                  else
                  {
                        //error type mismatch
                        ErrorHandler::semanticError(expr->line_no,"type mismatch between the operands of operator \'"+e->op+"\'");
                  }
            }
      }
      else if(auto e=dynamic_cast<UnaryExpr*>(expr))
      {
            if(e->type==DataType::UNKNOWN)
            {
                  operand_type=evaluate(e->operand);
                  if(operatorType.at(e->op).count(operand_type))
                  {
                        expr->type=operatorResultType.at(e->op);
                  }
                  else
                  {
                        //error unsupported operand type
                        ErrorHandler::semanticError(expr->line_no,"unsupported operand type for operator \'"+e->op+"\'");
                  }
            }
      }
      return expr->type;
}

void SemanticAnalyzer::handleStmt(Stmt* stmt)
{
      if(auto decl=dynamic_cast<DeclarationStmt*>(stmt))
      {
            handleDeclaration(decl);
      }
      else if(auto printstmt=dynamic_cast<PrintStmt*>(stmt))
      {
            handlePrint(printstmt);
      }
      else if(auto ifstmt=dynamic_cast<IfStmt*>(stmt))
      {
            handleIf(ifstmt);
      }
      else if(auto whilestmt=dynamic_cast<WhileStmt*>(stmt))
      {
            handleWhile(whilestmt);
      }
      else if(auto blockstmt=dynamic_cast<BlockStmt*>(stmt))
      {
            handleBlock(blockstmt);
      }
      else if(auto exprstmt=dynamic_cast<ExprStmt*>(stmt))
      {
            evaluate(exprstmt->expr);
      }
}

void SemanticAnalyzer::handleDeclaration(DeclarationStmt* decl)
{
      if(scopes.size()>0 && scopes.back().count(decl->name)==0)
      {
            if(decl->initializer==nullptr || decl->type==evaluate(decl->initializer))
            {
                  scopes.back()[decl->name]=decl->type;
            }
            else
            {
                  //error type mismatch
                  ErrorHandler::semanticError(decl->line_no,"type mismatch between the operands of operator \'=\'");
            }
      }
      else
      {
            //error
            if(scopes.back().at(decl->name)==decl->type)
            {
                  ErrorHandler::semanticError(decl->line_no,"redeclaration of \'"+decl->name+"\'");
            }
            else
            {
                  ErrorHandler::semanticError(decl->line_no,"conflicting declaration of \'"+decl->name+"\'");
            }
      } 
}

void SemanticAnalyzer::handlePrint(PrintStmt* printstmt)
{
      if(printstmt->content!=nullptr)
      {
            evaluate(printstmt->content);
      }
}

void SemanticAnalyzer::handleIf(IfStmt* stmt)
{
      if(evaluate(stmt->condition)!=DataType::BOOL)
      {
            //error
            ErrorHandler::semanticError(stmt->line_no,"condition must be of type bool");
      }
      handleStmt(stmt->If);
      if(stmt->Else!=nullptr)
      {
            handleStmt(stmt->Else);
      }
}

void SemanticAnalyzer::handleWhile(WhileStmt* stmt)
{
      if(evaluate(stmt->condition)!=DataType::BOOL)
      {
            //error
            ErrorHandler::semanticError(stmt->line_no,"condition must be of type bool");
      }
      handleStmt(stmt->body);
}

void SemanticAnalyzer::handleBlock(BlockStmt* stmt)
{
      size_t i;
      std::unordered_map<std::string, DataType> blk;
      scopes.push_back(blk);
      for(i=0; i<stmt->block.size(); i++)
      {
            handleStmt(stmt->block[i]);
      }
      scopes.pop_back();
}

void SemanticAnalyzer::analyze(std::vector<Stmt*>& program)
{
      size_t i;
      for(i=0; i<program.size(); i++)
      {
            handleStmt(program[i]);
      }
}