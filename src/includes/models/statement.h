#pragma once
#include "interfaces/i_statement_visitor.h"

class Statement {
public:
  virtual ~Statement() = default;
  virtual void accept(IStatementVisitor &visitor) const = 0;
};