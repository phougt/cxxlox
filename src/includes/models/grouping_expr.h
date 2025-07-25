#pragma once
#include "common.h"
#include "interfaces/i_expr_visitor.h"
#include "models/expr.h"
#include <memory>

class GroupingExpr : public Expr {
public:
  GroupingExpr(std::unique_ptr<Expr> expr) : expr{std::move(expr)} {}

  LoxValue accept(IExprVisitor &visitor) const override {
    return visitor.visit(*this);
  };

  std::unique_ptr<Expr> expr;
};