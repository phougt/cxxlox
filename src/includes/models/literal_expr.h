#pragma once
#include "common.h"
#include "interfaces/i_expr_visitor.h"
#include "models/expr.h"
#include "token.h"

class LiteralExpr : public Expr {
public:
  LiteralExpr(Token value) : value{value} {};

  LoxValue accept(IExprVisitor &visitor) const override {
    return visitor.visit(*this);
  }

  Token value;
};