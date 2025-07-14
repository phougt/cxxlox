#pragma once
#include "common.h"
#include "interfaces/ivisitor.h"
#include "models/expr.h"
#include "token.h"

class LiteralExpr : public Expr {
public:
  LiteralExpr(Token value) : value{value} {};

  LoxValue accept(const IVisitor &visitor) const override {
    return visitor.visit(*this);
  }

  Token value;
};