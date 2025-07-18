#pragma once
#include "common.h"
#include "interfaces/i_expr_visitor.h"
#include "interfaces/i_statement_visitor.h"
#include "models/assignment_expr.h"
#include "models/block_statement.h"
#include "models/expr.h"
#include "models/statement.h"
#include "models/var_statement.h"
#include "models/variable_expr.h"
#include "symbol_table.h"
#include <memory>
#include <vector>

class Interpreter : public IExprVisitor, IStatementVisitor {
public:
  Interpreter();
  ~Interpreter() = default;
  void interpret(const std::vector<std::unique_ptr<Statement>> &);

private:
  template <typename T> bool is(LoxValue value);
  bool isTruthy(LoxValue value);
  std::string loxValueToStr(LoxValue value);
  void execute(const Statement &expr);
  LoxValue evaluate(const Expr &expr);
  LoxValue visit(const BinaryExpr &expr) override;
  LoxValue visit(const UnaryExpr &expr) override;
  LoxValue visit(const LiteralExpr &expr) override;
  LoxValue visit(const GroupingExpr &expr) override;
  LoxValue visit(const VariableExpr &expr) override;
  LoxValue visit(const AssignmentExpr &expr) override;
  void visit(const ExprStatement &statement) override;
  void visit(const PrintStatement &statement) override;
  void visit(const VarStatement &statement) override;
  void visit(const BlockStatement &statement) override;

  std::unique_ptr<SymbolTable> symbolTable;
};