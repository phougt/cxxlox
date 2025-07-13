#pragma once
#include "interfaces/ivisitor.h"
#include "models/expr.h"
#include <memory>
#include <variant>

class GroupingExpr : public Expr {
public:
  GroupingExpr(std::unique_ptr<Expr> expr) : expr{std::move(expr)} {}
  std::variant<std::string, double> accept(IVisitor *visitor) override {
    return visitor->visit(this);
  };

  std::unique_ptr<Expr> expr;
};