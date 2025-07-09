#pragma once
#include "models/expr.h"
#include "models/token.h"

class BinaryExpr : public Expr {
public:
  Token op;
  Expr left;
  Expr right;
};