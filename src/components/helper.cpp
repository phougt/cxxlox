#include "helper.h"
#include "common.h"
#include <format>
#include <iostream>

namespace error {
void reportError(std::string reason, size_t line) {
  hasError = true;
  std::cerr << std::format("[Error]:Line {}. {}\n", line, reason);
}

void reportError(std::string reason, const Token &token) {
  switch (token.kind) {
  case TokenKind::IDENTIFIER:
  case TokenKind::STRING:
    reportError(std::format("{}, found '{}'", reason,
                            std::get<std::string>(token.literal)),
                token.line);
    break;
  case TokenKind::NUMBER:
    reportError(
        std::format("{}, found '{}'", reason, std::get<double>(token.literal)),
        token.line);
    break;
  default:
    reportError(
        std::format("{}, found '{}'", reason, tokenKindToStr(token.kind)),
        token.line);
    break;
  }
}

void reportRuntimeError(RuntimeException e) {
  hasRuntimeError = true;
  std::cerr << std::format("[Runtime Error]:Line {}. {}\n", e.token.line,
                           e.message);
};

void reportWarning(std::string reason, size_t line) {
  std::cerr << std::format("[Warning]:Line {}. {}.\n", reason, line);
}
} // namespace error

std::string tokenKindToStr(TokenKind kind) {
  switch (kind) {
  case TokenKind::LEFT_PAREN:
    return "(";
  case TokenKind::RIGHT_PAREN:
    return ")";
  case TokenKind::LEFT_BRACE:
    return "{";
  case TokenKind::RIGHT_BRACE:
    return "}";
  case TokenKind::COMMA:
    return ",";
  case TokenKind::DOT:
    return ".";
  case TokenKind::MINUS:
    return "-";
  case TokenKind::PLUS:
    return "+";
  case TokenKind::SEMICOLON:
    return ";";
  case TokenKind::SLASH:
    return "/";
  case TokenKind::STAR:
    return "*";
  case TokenKind::BANG:
    return "!";
  case TokenKind::BANG_EQUAL:
    return "!=";
  case TokenKind::EQUAL:
    return "=";
  case TokenKind::EQUAL_EQUAL:
    return "==";
  case TokenKind::GREATER:
    return ">";
  case TokenKind::GREATER_EQUAL:
    return ">=";
  case TokenKind::LESS:
    return "<";
  case TokenKind::LESS_EQUAL:
    return "<=";
  case TokenKind::IDENTIFIER:
    return "identifier";
  case TokenKind::STRING:
    return "string";
  case TokenKind::NUMBER:
    return "number";
  case TokenKind::AND:
    return "and";
  case TokenKind::CLASS:
    return "class";
  case TokenKind::ELSE:
    return "else";
  case TokenKind::FALSE:
    return "false";
  case TokenKind::FUN:
    return "fun";
  case TokenKind::FOR:
    return "for";
  case TokenKind::IF:
    return "if";
  case TokenKind::NIL:
    return "nil";
  case TokenKind::OR:
    return "or";
  case TokenKind::PRINT:
    return "print";
  case TokenKind::RETURN:
    return "return";
  case TokenKind::SUPER:
    return "super";
  case TokenKind::THIS:
    return "this";
  case TokenKind::TRUE:
    return "true";
  case TokenKind::VAR:
    return "var";
  case TokenKind::WHILE:
    return "while";
  case TokenKind::EOF_TOKEN:
    return "eof";
  default:
    return "";
  }
}