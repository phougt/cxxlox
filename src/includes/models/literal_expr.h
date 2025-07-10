#pragma once
#include "enums/token_kind.h"
#include "interfaces/ivisitor.h"
#include "models/expr.h"
#include <string>
#include <variant>

class LiteralExpr : public Expr {
public:
  LiteralExpr(TokenKind kind, std::string value) : kind{kind}, value{value} {}
  LiteralExpr(TokenKind kind, double value) : kind{kind}, value{value} {}
  void accept(IVisitor *visitor) override { visitor->visit(this); }

  TokenKind kind;
  std::variant<std::string, double> value;
};