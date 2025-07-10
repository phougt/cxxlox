#pragma once
#include "interfaces/ivisitor.h"
#include "models/expr.h"
#include <memory>

class GroupingExpr : public Expr {
public:
  GroupingExpr(std::unique_ptr<Expr> expr) : expr{std::move(expr)} {}
  void accept(IVisitor *visitor) override { visitor->visit(this); };

  std::unique_ptr<Expr> expr;
};