#pragma once
#include "common.h"
#include "interfaces/i_expr_visitor.h"

class Expr {
public:
  virtual ~Expr() = default;
  virtual LoxValue accept(IExprVisitor &visitor) const = 0;
};