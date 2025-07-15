#pragma once
#include "common.h"
#include "interfaces/i_expr_visitor.h"
#include "models/expr.h"
#include "token.h"
#include <memory>

class UnaryExpr : public Expr {
public:
  UnaryExpr(Token op, std::unique_ptr<Expr> expr)
      : op{op}, expr{std::move(expr)} {}

  LoxValue accept(IExprVisitor &visitor) const override {
    return visitor.visit(*this);
  }

  Token op;
  std::unique_ptr<Expr> expr;
};