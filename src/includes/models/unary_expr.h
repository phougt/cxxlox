#pragma once
#include "models/expr.h"
#include "models/token.h"

class UnaryExpr : public Expr {
public:
  Token op;
  Expr operand;
};