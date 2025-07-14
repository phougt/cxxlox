#include "interpreter.h"
#include "common.h"
#include "enums/token_kind.h"
#include "exceptions/runtime_exception.h"
#include "helper.h"
#include "models/binary_expr.h"
#include "models/grouping_expr.h"
#include "models/literal_expr.h"
#include "models/token.h"
#include "models/unary_expr.h"
#include <iostream>
#include <variant>

template <typename T> bool Interpreter::is(LoxValue value) const {
  return std::holds_alternative<T>(value);
}

bool Interpreter::isTruthy(LoxValue value) const {
  if (is<std::monostate>(value)) {
    return false;
  } else if (is<bool>(value)) {
    return std::get<bool>(value);
  }
  return true;
}

LoxValue Interpreter::evaluate(const Expr &expr) const {
  return expr.accept(*this);
}

std::string Interpreter::loxValueToStr(LoxValue value) const {
  if (is<std::string>(value)) {
    return std::get<std::string>(value);
  } else if (is<double>(value)) {
    return std::to_string(std::get<double>(value));
  } else if (is<bool>(value)) {
    return std::get<bool>(value) ? "true" : "false";
  }
  return "nil";
}

void Interpreter::interpret(const Expr &expr) const {
  try {
    auto result = evaluate(expr);
    std::cout << loxValueToStr(result) + '\n';
  } catch (RuntimeException e) {
    error::reportRuntimeError(e);
  }
}

LoxValue Interpreter::visit(const LiteralExpr &expr) const {
  switch (expr.value.kind) {
  case TokenKind::STRING:
  case TokenKind::NUMBER:
    return expr.value.literal;
  case TokenKind::TRUE:
    return true;
  case TokenKind::FALSE:
    return false;
  default:
    return std::monostate();
  }
}

LoxValue Interpreter::visit(const GroupingExpr &expr) const {
  return evaluate(*expr.expr.get());
}

LoxValue Interpreter::visit(const UnaryExpr &expr) const {
  auto right = evaluate(*expr.expr.get());

  if (expr.op.kind == TokenKind::MINUS) {
    if (is<double>(right)) {
      return LoxValue(-std::get<double>(right));
    }
    throw RuntimeException(expr.op, "Operand must be a number.");
  } else if (expr.op.kind == TokenKind::BANG) {
    return LoxValue(!isTruthy(right));
  }

  return std::monostate();
}

LoxValue Interpreter::visit(const BinaryExpr &expr) const {
  auto right = evaluate(*expr.right.get());
  auto left = evaluate(*expr.left.get());
  auto op = expr.op;

  switch (op.kind) {
  case TokenKind::PLUS: {
    if (is<double>(right) && is<double>(left)) {
      return LoxValue(std::get<double>(left) + std::get<double>(right));
    } else if (is<std::string>(right) && is<std::string>(left)) {
      return LoxValue(std::get<std::string>(left) +
                      std::get<std::string>(right));
    } else {
      throw RuntimeException(op, "Operands must be numbers or doubles.");
    }
  }
  case TokenKind::MINUS: {
    if (is<double>(right) && is<double>(left)) {
      return LoxValue(std::get<double>(left) - std::get<double>(right));
    } else {
      throw RuntimeException(op, "Operands must be numbers.");
    }
  }
  case TokenKind::SLASH: {
    if (is<double>(right) && is<double>(left)) {
      return LoxValue(std::get<double>(left) / std::get<double>(right));
    } else {
      throw RuntimeException(op, "Operands must be numbers.");
    }
  }
  case TokenKind::STAR: {
    if (is<double>(right) && is<double>(left)) {
      return LoxValue(std::get<double>(left) * std::get<double>(right));
    } else {
      throw RuntimeException(op, "Operands must be numbers.");
    }
  }
  case TokenKind::GREATER: {
    if (is<double>(right) && is<double>(left)) {
      return LoxValue(std::get<double>(left) > std::get<double>(right));
    } else {
      throw RuntimeException(op, "Operands must be numbers.");
    }
  }
  case TokenKind::GREATER_EQUAL: {
    if (is<double>(right) && is<double>(left)) {
      return LoxValue(std::get<double>(left) >= std::get<double>(right));
    } else {
      throw RuntimeException(op, "Operands must be numbers.");
    }
  }
  case TokenKind::LESS: {
    if (is<double>(right) && is<double>(left)) {
      return LoxValue(std::get<double>(left) < std::get<double>(right));
    } else {
      throw RuntimeException(op, "Operands must be numbers.");
    }
  }
  case TokenKind::LESS_EQUAL: {
    if (is<double>(right) && is<double>(left)) {
      return LoxValue(std::get<double>(left) <= std::get<double>(right));
    } else {
      throw RuntimeException(op, "Operands must be numbers.");
    }
  }
  case TokenKind::EQUAL_EQUAL: {
    return LoxValue(left == right);
  }
  case TokenKind::BANG_EQUAL: {
    return LoxValue(left != right);
  }
  }

  return std::monostate();
}