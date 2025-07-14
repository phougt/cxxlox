#pragma once
#include "common.h"
#include <enums/token_kind.h>

struct Token {
  TokenKind kind;
  LoxValue literal;
  size_t line;
};