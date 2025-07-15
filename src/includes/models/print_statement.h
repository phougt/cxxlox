#pragma once
#include "expr.h"
#include "statement.h"
#include <memory>

class PrintStatement : public Statement {
public:
  PrintStatement(std::unique_ptr<Expr> expr) : expr{std::move(expr)} {}
  void accept(IStatementVisitor &visitor) const override {
    visitor.visit(*this);
  }

  std::unique_ptr<Expr> expr;
};