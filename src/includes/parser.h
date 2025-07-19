#pragma once
#include "enums/token_kind.h"
#include "exceptions/parser_exception.h"
#include "models/expr.h"
#include "models/token.h"
#include <cstddef>
#include <memory>
#include <models/statement.h>
#include <vector>

class Parser {
public:
  Parser();
  Parser(std::vector<Token> &&tokens);
  std::vector<std::unique_ptr<Statement>> parse();

private:
  const Token &peek(int count = 1);
  const Token &advance();
  bool expect(TokenKind kind);
  bool advanceIfExpect(TokenKind kind);
  const Token &throwOrAdvanceIfExpect(TokenKind kind, std::string reason);
  bool isEOF();
  void synchronize();
  ParserException reportAndCreateError(std::string, const Token &);
  std::unique_ptr<Statement> declaration();
  std::unique_ptr<Statement> varDeclaration();
  std::unique_ptr<Statement> statement();
  std::unique_ptr<Statement> ifStatement();
  std::unique_ptr<Statement> whileStatement();
  std::unique_ptr<Statement> forStatement();
  std::unique_ptr<Statement> exprStatement();
  std::unique_ptr<Statement> printStatement();
  std::unique_ptr<Statement> blockStatement();
  std::unique_ptr<Expr> expression();
  std::unique_ptr<Expr> assignment();
  std::unique_ptr<Expr> logicalOr();
  std::unique_ptr<Expr> logicalAnd();
  std::unique_ptr<Expr> equality();
  std::unique_ptr<Expr> comparison();
  std::unique_ptr<Expr> term();
  std::unique_ptr<Expr> factor();
  std::unique_ptr<Expr> unary();
  std::unique_ptr<Expr> primary();

  std::vector<Token> tokens;
  size_t current{0};
};