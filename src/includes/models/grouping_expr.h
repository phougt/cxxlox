#pragma once
#include "common.h"
#include "interfaces/ivisitor.h"
#include "models/expr.h"
#include <memory>

class GroupingExpr : public Expr {
public:
  GroupingExpr(std::unique_ptr<Expr> expr) : expr{std::move(expr)} {}

  LoxValue accept(const IVisitor &visitor) const override {
    return visitor.visit(*this);
  };

  std::unique_ptr<Expr> expr;
};