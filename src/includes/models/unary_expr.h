#pragma once
#include "interfaces/ivisitor.h"
#include "models/expr.h"
#include "token.h"
#include <memory>
#include <variant>

class UnaryExpr : public Expr {
public:
  UnaryExpr(Token op, std::unique_ptr<Expr> expr)
      : op{op}, expr{std::move(expr)} {}

  std::variant<std::string, double> accept(IVisitor *visitor) override {
    return visitor->visit(this);
  }

  Token op;
  std::unique_ptr<Expr> expr;
};