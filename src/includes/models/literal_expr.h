#pragma once
#include "interfaces/ivisitor.h"
#include "models/expr.h"
#include "token.h"

class LiteralExpr : public Expr {
public:
  LiteralExpr(Token value) : value{value} {};
  void accept(IVisitor *visitor) override { visitor->visit(this); }

  Token value;
};