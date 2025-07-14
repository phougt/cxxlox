#pragma once
#include "common.h"
#include "interfaces/ivisitor.h"
#include "models/expr.h"

class Interpreter : public IVisitor {
public:
  Interpreter() = default;
  ~Interpreter() = default;
  void interpret(const Expr &expr) const;

private:
  template <typename T> bool is(LoxValue value) const;
  bool isTruthy(LoxValue value) const;
  std::string loxValueToStr(LoxValue value) const;
  LoxValue evaluate(const Expr &expr) const;
  LoxValue visit(const BinaryExpr &expr) const override;
  LoxValue visit(const UnaryExpr &expr) const override;
  LoxValue visit(const LiteralExpr &expr) const override;
  LoxValue visit(const GroupingExpr &expr) const override;
};