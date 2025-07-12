#pragma once
#include "enums/token_kind.h"
#include "exceptions/parser_exception.h"
#include "models/expr.h"
#include "models/token.h"
#include <cstddef>
#include <memory>
#include <vector>

class Parser {
public:
  Parser();
  Parser(std::vector<Token> &tokens);

private:
  const Token &peek(int count = 1);
  const Token &advance();
  bool expect(TokenKind kind);
  bool advanceIfExpect(TokenKind kind);
  void throwOrAdvanceIfExpect(TokenKind kind, std::string reason);
  bool isEOF();
  ParserException reportAndCreateError(std::string, const Token &);
  std::unique_ptr<Expr> expression();
  std::unique_ptr<Expr> equality();
  std::unique_ptr<Expr> comparison();
  std::unique_ptr<Expr> term();
  std::unique_ptr<Expr> factor();
  std::unique_ptr<Expr> unary();
  std::unique_ptr<Expr> primary();

  std::vector<Token> tokens;
  size_t current;
};