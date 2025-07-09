#pragma once
#include <enums/token_kind.h>
#include <string>
#include <variant>

struct Token {
  TokenKind kind;
  std::variant<double, std::string> literal;
  size_t line;
};