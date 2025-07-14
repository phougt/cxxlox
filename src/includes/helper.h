#pragma once
#include "enums/token_kind.h"
#include "exceptions/runtime_exception.h"
#include "models/token.h"
#include <string>

namespace error {
void reportError(std::string reason, size_t line);
void reportError(std::string, const Token &token);
void reportRuntimeError(RuntimeException e);
void reportWarning(std::string reason, size_t line);
}

std::string tokenKindToStr(TokenKind kind);