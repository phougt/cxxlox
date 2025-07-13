#pragma once
#include "interfaces/ivisitor.h"
#include "models/expr.h"
#include "token.h"
#include <variant>

class LiteralExpr : public Expr {
public:
  LiteralExpr(Token value) : value{value} {};
  std::variant<std::string, double> accept(IVisitor *visitor) override {
    return visitor->visit(this);
  }

  Token value;
};