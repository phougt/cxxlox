#pragma once
#include "enums/token_kind.h"
#include "models/token.h"
#include <string>

namespace error {
void reportError(std::string reason, size_t line);
void reportError(std::string, const Token &token);
void reportWarning(std::string reason, size_t line);
}

std::string tokenKindToStr(TokenKind kind);