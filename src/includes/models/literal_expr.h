#pragma once
#include "models/expr.h"
#include <string>
#include <variant>

class LiteralExpr : public Expr {
public:
    std::variant<std::string, double> value;
};