#pragma once
#include "interfaces/ivisitor.h"

class Expr {
public:
  virtual ~Expr() = default;
  virtual void accept(IVisitor *visitor) = 0;
};