#pragma once
#include <string>
#include <variant>


class BinaryExpr;
class UnaryExpr;
class LiteralExpr;
class GroupingExpr;

class IVisitor {
public:
  virtual std::variant<std::string, double> visit(const BinaryExpr *expr) = 0;
  virtual std::variant<std::string, double> visit(const UnaryExpr *expr) = 0;
  virtual std::variant<std::string, double> visit(const LiteralExpr *expr) = 0;
  virtual std::variant<std::string, double> visit(const GroupingExpr *expr) = 0;
};