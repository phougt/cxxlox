#include "parser.h"
#include "enums/token_kind.h"
#include "helper.h"
#include "models/binary_expr.h"
#include "models/literal_expr.h"
#include "models/unary_expr.h"
#include <memory>

Parser::Parser() {}
Parser::Parser(std::vector<Token> &&tokens) : tokens{std::move(tokens)} {}

std::unique_ptr<Expr> Parser::parse() {
  try {
    return expression();
  } catch (ParserException &e) {
    return nullptr;
  }
}

bool Parser::expect(TokenKind kind) {
  if (peek().kind == kind) {
    return true;
  }
  return false;
}

bool Parser::advanceIfExpect(TokenKind kind) {
  if (peek().kind == kind) {
    advance();
    return true;
  }
  return false;
}

const Token &Parser::peek(int count) {
  if (current + count - 1 >= tokens.size() || count < 0) {
    return tokens.back();
  }
  return tokens.at(current + count - 1);
}

const Token &Parser::advance() {
  if (isEOF()) {
    return tokens.back();
  }
  return tokens.at(current++);
}

bool Parser::isEOF() {
  const Token &temp = peek();
  if (temp.kind == TokenKind::EOF_TOKEN) {
    return true;
  }
  return false;
}

ParserException Parser::reportAndCreateError(std::string reason,
                                             const Token &token) {
  error::reportError(reason, token);
  return ParserException();
}

void Parser::throwOrAdvanceIfExpect(TokenKind kind, std::string reasonToThrow) {
  if (expect(kind)) {
    advance();
  } else {
    throw reportAndCreateError(reasonToThrow, peek());
  }
}

std::unique_ptr<Expr> Parser::expression() { return equality(); }

std::unique_ptr<Expr> Parser::equality() {
  auto tempExpr{comparison()};

  while (expect(TokenKind::BANG_EQUAL) || expect(TokenKind::EQUAL_EQUAL)) {
    const Token &op = advance();
    auto expr{comparison()};
    tempExpr =
        std::make_unique<BinaryExpr>(op, std::move(tempExpr), std::move(expr));
  }

  return tempExpr;
}

std::unique_ptr<Expr> Parser::comparison() {
  auto tempExpr{term()};

  while (expect(TokenKind::GREATER) || expect(TokenKind::GREATER_EQUAL) ||
         expect(TokenKind::LESS) || expect(TokenKind::LESS_EQUAL)) {
    const Token &op = advance();
    auto expr{term()};
    tempExpr =
        std::make_unique<BinaryExpr>(op, std::move(tempExpr), std::move(expr));
  }

  return tempExpr;
}

std::unique_ptr<Expr> Parser::term() {
  auto tempExpr{factor()};

  while (expect(TokenKind::MINUS) || expect(TokenKind::PLUS)) {
    const Token &op = advance();
    auto expr{factor()};
    tempExpr =
        std::make_unique<BinaryExpr>(op, std::move(tempExpr), std::move(expr));
  }

  return tempExpr;
}

std::unique_ptr<Expr> Parser::factor() {
  auto tempExpr{unary()};

  while (expect(TokenKind::SLASH) || expect(TokenKind::STAR)) {
    const Token &op = advance();
    auto expr{unary()};
    tempExpr =
        std::make_unique<BinaryExpr>(op, std::move(tempExpr), std::move(expr));
  }

  return tempExpr;
}

std::unique_ptr<Expr> Parser::unary() {
  if (expect(TokenKind::BANG) || expect(TokenKind::MINUS)) {
    const Token &op = advance();
    auto expr{unary()};
    return std::make_unique<UnaryExpr>(op, std::move(expr));
  }

  auto tempExpr{primary()};
  return tempExpr;
}

std::unique_ptr<Expr> Parser::primary() {
  bool isValidPrimary = expect(TokenKind::STRING) ||
                        expect(TokenKind::NUMBER) || expect(TokenKind::TRUE) ||
                        expect(TokenKind::FALSE) || expect(TokenKind::NIL);
  if (isValidPrimary) {
    return std::make_unique<LiteralExpr>(advance());
  }

  if (expect(TokenKind::LEFT_PAREN)) {
    advance();
    auto expr{expression()};
    throwOrAdvanceIfExpect(TokenKind::RIGHT_PAREN,
                           "Expected ')' after expression.");
    return expr;
  }

  throw reportAndCreateError("Expected expression.", peek());
}