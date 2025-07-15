#pragma once
#include "common.h"
#include "interfaces/i_expr_visitor.h"
#include "interfaces/i_statement_visitor.h"
#include "models/expr.h"
#include "models/statement.h"
#include "models/var_statement.h"
#include "models/variable_expr.h"
#include <map>
#include <memory>
#include <vector>

class Interpreter : public IExprVisitor, IStatementVisitor {
public:
  Interpreter() = default;
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
  void visit(const ExprStatement &statement) override;
  void visit(const PrintStatement &statement) override;
  void visit(const VarStatement &statement) override;

  std::map<std::string, LoxValue> symbolTable;
};