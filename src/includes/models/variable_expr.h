#pragma once
#include "common.h"
#include "interfaces/i_expr_visitor.h"
#include "models/expr.h"
#include "models/token.h"

class VariableExpr : public Expr {
public:
  VariableExpr(Token name) : name{name} {}

  LoxValue accept(IExprVisitor &visitor) const override {
    return visitor.visit(*this);
  };

  Token name;
};