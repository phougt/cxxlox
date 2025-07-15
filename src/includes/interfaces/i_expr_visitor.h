#pragma once
#include "common.h"

class BinaryExpr;
class UnaryExpr;
class LiteralExpr;
class GroupingExpr;
class VariableExpr;

class IExprVisitor {
public:
  virtual LoxValue visit(const BinaryExpr &expr) = 0;
  virtual LoxValue visit(const UnaryExpr &expr) = 0;
  virtual LoxValue visit(const LiteralExpr &expr) = 0;
  virtual LoxValue visit(const GroupingExpr &expr) = 0;
  virtual LoxValue visit(const VariableExpr &expr) = 0;
};