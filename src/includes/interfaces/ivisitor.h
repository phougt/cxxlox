#pragma once
#include "common.h"

class BinaryExpr;
class UnaryExpr;
class LiteralExpr;
class GroupingExpr;

class IVisitor {
public:
  virtual LoxValue visit(const BinaryExpr &expr) const = 0;
  virtual LoxValue visit(const UnaryExpr &expr) const = 0;
  virtual LoxValue visit(const LiteralExpr &expr) const = 0;
  virtual LoxValue visit(const GroupingExpr &expr) const = 0;
};