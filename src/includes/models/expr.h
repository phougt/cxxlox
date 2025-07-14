#pragma once
#include "common.h"
#include "interfaces/ivisitor.h"

class Expr {
public:
  virtual ~Expr() = default;
  virtual LoxValue accept(const IVisitor &visitor) const = 0;
};