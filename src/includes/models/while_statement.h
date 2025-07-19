#pragma once
#include "expr.h"
#include "statement.h"
#include <memory>

class WhileStatement : public Statement {
public:
  WhileStatement(std::unique_ptr<Expr> condition,
                 std::unique_ptr<Statement> &&statement)
      : condition{std::move(condition)}, statement(std::move(statement)) {}

  void accept(IStatementVisitor &visitor) const override {
    visitor.visit(*this);
  }

  std::unique_ptr<Statement> statement;
  std::unique_ptr<Expr> condition;
};