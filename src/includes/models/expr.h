#pragma once
#include "interfaces/ivisitor.h"
#include <variant>

class Expr {
public:
  virtual ~Expr() = default;
  virtual std::variant<std::string, double> accept(IVisitor *visitor) = 0;
};