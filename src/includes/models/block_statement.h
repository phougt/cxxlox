#pragma once
#include "models/statement.h"
#include "statement.h"
#include <memory>
#include <vector>

class BlockStatement : public Statement {
public:
  BlockStatement(std::vector<std::unique_ptr<Statement>> &&statements)
      : statements{std::move(statements)} {}
  void accept(IStatementVisitor &visitor) const override {
    visitor.visit(*this);
  }

  std::vector<std::unique_ptr<Statement>> statements;
};