#include "interpreter.h"
#include "common.h"
#include "enums/token_kind.h"
#include "exceptions/runtime_exception.h"
#include "helper.h"
#include "interfaces/i_expr_visitor.h"
#include "interfaces/i_statement_visitor.h"
#include "models/binary_expr.h"
#include "models/expr_statement.h"
#include "models/grouping_expr.h"
#include "models/literal_expr.h"
#include "models/print_statement.h"
#include "models/statement.h"
#include "models/token.h"
#include "models/unary_expr.h"
#include <iostream>
#include <memory>
#include <variant>

template <typename T> bool Interpreter::is(LoxValue value) {
  return std::holds_alternative<T>(value);
}

bool Interpreter::isTruthy(LoxValue value) {
  if (is<std::monostate>(value)) {
    return false;
  } else if (is<bool>(value)) {
    return std::get<bool>(value);
  }
  return true;
}

LoxValue Interpreter::evaluate(const Expr &expr) { return expr.accept(*this); }

std::string Interpreter::loxValueToStr(LoxValue value) {
  if (is<std::string>(value)) {
    return std::get<std::string>(value);
  } else if (is<double>(value)) {
    return std::to_string(std::get<double>(value));
  } else if (is<bool>(value)) {
    return std::get<bool>(value) ? "true" : "false";
  }
  return "nil";
}

void Interpreter::interpret(
    const std::vector<std::unique_ptr<Statement>> &statements) {
  try {
    for (const auto &stmt : statements) {
      execute(*stmt.get());
    }
  } catch (RuntimeException e) {
    error::reportRuntimeError(e);
  }
}

void Interpreter::execute(const Statement &statement) {
  statement.accept(*this);
}

LoxValue Interpreter::visit(const LiteralExpr &expr) {
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

LoxValue Interpreter::visit(const GroupingExpr &expr) {
  return evaluate(*expr.expr.get());
}

LoxValue Interpreter::visit(const UnaryExpr &expr) {
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

LoxValue Interpreter::visit(const BinaryExpr &expr) {
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

LoxValue Interpreter::visit(const VariableExpr &expr) {
  std::string varName = std::get<std::string>(expr.name.literal);

  if (symbolTable.contains(varName)) {
    return symbolTable[varName];
  }

  throw RuntimeException(expr.name, "Undefined variable '" + varName + "'.");
}

void Interpreter::visit(const ExprStatement &statement) {
  evaluate(*statement.expr.get());
}

void Interpreter::visit(const PrintStatement &statement) {
  auto value = evaluate(*statement.expr.get());
  std::cout << loxValueToStr(value) + '\n';
}

void Interpreter::visit(const VarStatement &statement) {
  LoxValue value{std::monostate()};
  std::string varName = std::get<std::string>(statement.name.literal);

  if (statement.initializer != nullptr) {
    value = evaluate(*statement.initializer.get());
  }

  symbolTable.insert_or_assign(varName, value);
}