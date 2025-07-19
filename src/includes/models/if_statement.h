#pragma once
#include "expr.h"
#include "statement.h"
#include <memory>
#include <vector>

class IfStatement : public Statement {
public:
  IfStatement(std::unique_ptr<Expr> condition,
              std::unique_ptr<Statement> &&thenStatement,
              std::unique_ptr<Statement> &&elseStatement)
      : condition{std::move(condition)},
        thenStatement(std::move(thenStatement)),
        elseStatement(std::move(elseStatement)) {}

  void accept(IStatementVisitor &visitor) const override {
    visitor.visit(*this);
  }

  std::unique_ptr<Statement> thenStatement;
  std::unique_ptr<Statement> elseStatement;
  std::unique_ptr<Expr> condition;
};