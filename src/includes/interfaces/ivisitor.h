#pragma once

class BinaryExpr;
class UnaryExpr;
class LiteralExpr;
class GroupingExpr;

class IVisitor {
public:
  virtual void visit(const BinaryExpr *expr) = 0;
  virtual void visit(const UnaryExpr *expr) = 0;
  virtual void visit(const LiteralExpr *expr) = 0;
  virtual void visit(const GroupingExpr *expr) = 0;
};