#pragma once
#include "Lexer.h"
enum class DataType:int
{
      INT, BOOL, UNKNOWN
};

struct Expr
{
      int line_no;
      DataType type;
      Expr(int, DataType type=DataType::UNKNOWN);
      virtual ~Expr()=default;
};

struct Stmt
{
      int line_no;
      Stmt(int);
      virtual void print()=0;
      virtual ~Stmt()=default;
};

//Types of expressions
struct NumberExpr:public Expr
{
      int val;
      NumberExpr(int, int);
};
struct VarExpr:public Expr
{
      std::string name;
      VarExpr(int, const std::string&);
};
struct InputExpr:public Expr
{
      InputExpr(int);
};
struct BoolExpr:public Expr
{
      bool val;
      BoolExpr(int, bool);
};
struct BinaryExpr:public Expr
{
      std::string op;
      Expr* left;
      Expr* right;
      BinaryExpr(int, const std::string&, Expr* l=nullptr, Expr* r=nullptr);
      ~BinaryExpr();
};
struct UnaryExpr:public Expr
{
      std::string op;
      Expr* operand;
      UnaryExpr(int, const std::string&, Expr*);
      ~UnaryExpr();
};

//Types of statements
struct DeclarationStmt:public Stmt
{
      DataType type;
      std::string name;
      Expr* initializer;
      DeclarationStmt(int, DataType, const std::string&, Expr*);
      void print() override;
      ~DeclarationStmt();
};
struct PrintStmt:public Stmt
{
      Expr* content;
      PrintStmt(int, Expr*);
      void print() override;
      ~PrintStmt();
};
struct WhileStmt:public Stmt
{
      Expr* condition;
      Stmt* body;
      WhileStmt(int, Expr*, Stmt*);
      void print() override;
      ~WhileStmt();
};
struct IfStmt:public Stmt
{
      Expr* condition;
      Stmt* If;
      Stmt* Else;
      IfStmt(int, Expr*, Stmt*, Stmt*);
      void print() override;
      ~IfStmt();
};
struct BlockStmt:public Stmt
{
      std::vector<Stmt*> block;
      BlockStmt(int, std::vector<Stmt*>);
      void print() override;
      ~BlockStmt();
};
struct ExprStmt:public Stmt
{
      Expr* expr;
      ExprStmt(int, Expr*);
      void print() override;
      ~ExprStmt();
};

class AST
{
      private:
            Expr* root;
      public:
            AST(Expr*);
            static void print(Expr*, std::string ind="");
            ~AST();
};