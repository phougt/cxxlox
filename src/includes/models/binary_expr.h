#pragma once
#include "common.h"
#include "interfaces/ivisitor.h"
#include "models/expr.h"
#include "token.h"
#include <memory>

class BinaryExpr : public Expr {
public:
  BinaryExpr(Token op, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
      : op{op}, left{std::move(left)}, right{std::move(right)} {}

  LoxValue accept(const IVisitor &visitor) const override {
    return visitor.visit(*this);
  }

  Token op;
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;
};