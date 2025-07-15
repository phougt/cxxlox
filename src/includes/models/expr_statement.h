#pragma once
#include "expr.h"
#include "statement.h"
#include <memory>

class ExprStatement : public Statement {
public:
  ExprStatement(std::unique_ptr<Expr> expr) : expr{std::move(expr)} {}
  void accept(IStatementVisitor &visitor) const override {
    visitor.visit(*this);
  }

  std::unique_ptr<Expr> expr;
};