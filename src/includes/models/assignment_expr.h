#pragma once
#include "common.h"
#include "interfaces/i_expr_visitor.h"
#include "models/expr.h"
#include "token.h"
#include <memory>

class AssignmentExpr : public Expr {
public:
  AssignmentExpr(Token name, std::unique_ptr<Expr> value)
      : name{name}, value{std::move(value)} {}

  LoxValue accept(IExprVisitor &visitor) const override {
    return visitor.visit(*this);
  }

  Token name;
  std::unique_ptr<Expr> value;
};