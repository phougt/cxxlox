#pragma once
#include "expr.h"
#include "models/token.h"
#include "statement.h"
#include <memory>

class VarStatement : public Statement {
public:
  VarStatement(Token name, std::unique_ptr<Expr> initializer)
      : initializer{std::move(initializer)}, name{name} {}

  void accept(IStatementVisitor &visitor) const override {
    visitor.visit(*this);
  }

  Token name;
  std::unique_ptr<Expr> initializer;
};