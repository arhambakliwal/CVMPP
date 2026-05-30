#include "../include/AST.h"
Expr::Expr(int line_no, DataType type):line_no(line_no), type(type) {}

NumberExpr::NumberExpr(int line_no, int val):Expr(line_no, DataType::INT), val(val) {}

BoolExpr::BoolExpr(int line_no, bool val):Expr(line_no, DataType::BOOL), val(val) {}

BinaryExpr::BinaryExpr(int line_no, const std::string& op, Expr* l, Expr* r):Expr(line_no), op(op), left(l), right(r) {}

UnaryExpr::UnaryExpr(int line_no, const std::string& op, Expr* o):Expr(line_no), op(op), operand(o) {}

VarExpr::VarExpr(int line_no, const std::string& name):Expr(line_no), name(name) {}

InputExpr::InputExpr(int line_no):Expr(line_no, DataType::INT){}

BinaryExpr::~BinaryExpr()
{
      delete left;
      delete right;
}

UnaryExpr::~UnaryExpr()
{
      delete operand;
}

Stmt::Stmt(int line_no):line_no(line_no) {}

DeclarationStmt::DeclarationStmt(int line_no, DataType type, const std::string& name, Expr* init):Stmt(line_no), type(type), name(name), initializer(init){}

ExprStmt::ExprStmt(int line_no, Expr* expr):Stmt(line_no), expr(expr){}

PrintStmt::PrintStmt(int line_no, Expr* content):Stmt(line_no), content(content) {}

IfStmt::IfStmt(int line_no, Expr* cond, Stmt* If, Stmt* Else):Stmt(line_no), condition(cond), If(If), Else(Else) {}

WhileStmt::WhileStmt(int line_no, Expr* cond, Stmt* body):Stmt(line_no), condition(cond), body(body) {}

BlockStmt::BlockStmt(int line_no, const std::vector<Stmt*>& blk):Stmt(line_no), block(blk) {}

void DeclarationStmt::print()
{
      std::cout<<"Declaration Stmt\n";
      if(type==DataType::INT)
      {
            std::cout<<"Type: INT\n";
      }
      else if(type==DataType::BOOL)
      {
            std::cout<<"Type: BOOL\n";
      }
      std::cout<<"Name: "<<name<<std::endl;
      std::cout<<"Initializer:\n";
      AST::print(initializer);
      std::cout<<std::endl;
}

void ExprStmt::print()
{
      std::cout<<"Expression Statement:\n";
      AST::print(expr);
      std::cout<<std::endl;
}

void PrintStmt::print()
{
      std::cout<<"Print Statement:\n";
      std::cout<<"Content:\n";
      AST::print(content);
      std::cout<<std::endl;
}

void IfStmt::print()
{
      std::cout<<"If Statement:\n";
      std::cout<<"Condition:\n";
      AST::print(condition);
      std::cout<<"If Part:\n";
      If->print();
      std::cout<<"Else Part:\n";
      if(Else!=nullptr)
      {
            Else->print();
      }
      else
      {
            std::cout<<"_\n";
      }
      std::cout<<std::endl;
}

void WhileStmt::print()
{
      std::cout<<"While Statement:\n";
      std::cout<<"Condition:\n";
      AST::print(condition);
      std::cout<<"Body:\n";
      if(body!=nullptr)
      {
            body->print();
      }
      else
      {
            std::cout<<"_\n";
      }
      std::cout<<std::endl;
}

void BlockStmt::print()
{
      size_t i;
      for(i=0;  i<block.size(); i++)
      {
            block[i]->print();
      }
      std::cout<<std::endl;
}

DeclarationStmt::~DeclarationStmt()
{
      delete initializer;
}

PrintStmt::~PrintStmt()
{
      delete content;
}

IfStmt::~IfStmt()
{
      delete condition;
      delete If;
      delete Else;
}

WhileStmt::~WhileStmt()
{
      delete condition;
      delete body;
}

ExprStmt::~ExprStmt()
{
      delete expr;
}

BlockStmt::~BlockStmt()
{
      size_t i;
      for(i=0; i<block.size(); i++)
      {
            if(block[i]!=nullptr)
            {
                  delete block[i];
            }
      }
}

void AST::print(Expr* node, std::string ind)
{
      if(node!=nullptr)
      {
            if(auto binary=dynamic_cast<BinaryExpr*>(node))
            {
                  std::cout<<ind<<binary->op<<std::endl;
                  ind.push_back('\t');
                  print(binary->left, ind);
                  print(binary->right, ind);
            }
            else if(auto unary=dynamic_cast<UnaryExpr*>(node))
            {
                  std::cout<<ind<<unary->op<<std::endl;
                  ind.push_back('\t');
                  print(unary->operand, ind);
            }
            else if(auto v=dynamic_cast<VarExpr*>(node))
            {
                  std::cout<<ind<<v->name<<std::endl;
            }
            else if(auto n=dynamic_cast<NumberExpr*>(node))
            {
                  std::cout<<ind<<n->val<<std::endl;
            }
            else if(auto be=dynamic_cast<BoolExpr*>(node))
            {
                  std::cout<<ind<<be->val<<std::endl;
            }
            else if(dynamic_cast<InputExpr*>(node))
            {
                  std::cout<<ind<<"input()"<<std::endl;
            }
      }
      else
      {
            std::cout<<ind<<"_"<<std::endl;
      }
}

AST::AST(Expr* r):root(r){}

AST::~AST()
{
      delete root;
}