#pragma once
#include "interfaces/ivisitor.h"
#include "models/expr.h"
#include "token.h"
#include <memory>
#include <variant>

class BinaryExpr : public Expr {
public:
  BinaryExpr(Token op, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
      : op{op}, left{std::move(left)}, right{std::move(right)} {}

  std::variant<std::string, double> accept(IVisitor *visitor) override {
    return visitor->visit(this);
  }

  Token op;
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;
};