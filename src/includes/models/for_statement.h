#pragma once
#include "expr.h"
#include "models/statement.h"
#include "statement.h"
#include <memory>

class ForStatement : public Statement {
public:
  ForStatement(std::unique_ptr<Statement> initialization,
               std::unique_ptr<Expr> condition, std::unique_ptr<Expr> update,
               std::unique_ptr<Statement> body)
      : condition{std::move(condition)},
        initialization(std::move(initialization)), update(std::move(update)),
        body{std::move(body)} {}

  void accept(IStatementVisitor &visitor) const override {
    visitor.visit(*this);
  }

  std::unique_ptr<Statement> initialization;
  std::unique_ptr<Expr> condition;
  std::unique_ptr<Expr> update;
  std::unique_ptr<Statement> body;
};