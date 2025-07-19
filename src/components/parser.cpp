#include "parser.h"
#include "enums/token_kind.h"
#include "exceptions/parser_exception.h"
#include "helper.h"
#include "models/assignment_expr.h"
#include "models/binary_expr.h"
#include "models/block_statement.h"
#include "models/expr_statement.h"
#include "models/if_statement.h"
#include "models/literal_expr.h"
#include "models/logical_expr.h"
#include "models/print_statement.h"
#include "models/statement.h"
#include "models/unary_expr.h"
#include "models/var_statement.h"
#include "models/variable_expr.h"
#include <memory>

Parser::Parser() {}
Parser::Parser(std::vector<Token> &&tokens) : tokens{std::move(tokens)} {}

std::vector<std::unique_ptr<Statement>> Parser::parse() {
  std::vector<std::unique_ptr<Statement>> statements{};

  while (!isEOF()) {
    statements.push_back(declaration());
  }

  return std::move(statements);
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

const Token &Parser::throwOrAdvanceIfExpect(TokenKind kind,
                                            std::string reasonToThrow) {
  if (expect(kind)) {
    return advance();
  } else {
    throw reportAndCreateError(reasonToThrow, peek());
  }
}

std::unique_ptr<Expr> Parser::expression() { return assignment(); }

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

std::unique_ptr<Expr> Parser::logicalOr() {
  auto leftExpr{logicalAnd()};

  if (expect(TokenKind::OR)) {
    Token op{advance()};
    auto rightExpr{logicalAnd()};
    return std::make_unique<LogicalExpr>(op, std::move(leftExpr),
                                         std::move(rightExpr));
  }

  return leftExpr;
}

std::unique_ptr<Expr> Parser::logicalAnd() {
  auto leftExpr{equality()};

  if (expect(TokenKind::AND)) {
    Token op{advance()};
    auto rightExpr{equality()};
    return std::make_unique<LogicalExpr>(op, std::move(leftExpr),
                                         std::move(rightExpr));
  }

  return leftExpr;
}

std::unique_ptr<Expr> Parser::assignment() {
  const Token &tempToken = peek();
  auto tempExpr{logicalOr()};

  if (advanceIfExpect(TokenKind::EQUAL)) {
    auto rightHandSideExpr{assignment()};
    const VariableExpr *variableExpr =
        dynamic_cast<VariableExpr *>(tempExpr.get());

    if (variableExpr != nullptr) {
      return std::make_unique<AssignmentExpr>(variableExpr->name,
                                              std::move(rightHandSideExpr));
    }
    throw reportAndCreateError("Expected variable name", tempToken);
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

  if (expect(TokenKind::IDENTIFIER)) {
    return std::make_unique<VariableExpr>(advance());
  }

  if (expect(TokenKind::LEFT_PAREN)) {
    advance();
    auto expr{expression()};
    throwOrAdvanceIfExpect(TokenKind::RIGHT_PAREN,
                           "Expected ')' after expression");
    return expr;
  }

  throw reportAndCreateError("Expected expression", peek());
}

std::unique_ptr<Statement> Parser::statement() {
  if (advanceIfExpect(TokenKind::PRINT)) {
    return printStatement();
  } else if (advanceIfExpect(TokenKind::LEFT_BRACE)) {
    return blockStatement();
  } else if (advanceIfExpect(TokenKind::IF)) {
    return ifStatement();
  } else {
    return exprStatement();
  }
}

std::unique_ptr<Statement> Parser::exprStatement() {
  auto expr{expression()};
  throwOrAdvanceIfExpect(TokenKind::SEMICOLON, "Expected ';' after expression");
  return std::make_unique<ExprStatement>(std::move(expr));
}

std::unique_ptr<Statement> Parser::printStatement() {
  auto expr{expression()};
  throwOrAdvanceIfExpect(TokenKind::SEMICOLON, "Expected ';' after expression");
  return std::make_unique<PrintStatement>(std::move(expr));
}

std::unique_ptr<Statement> Parser::blockStatement() {
  std::vector<std::unique_ptr<Statement>> statements{};

  while (!expect(TokenKind::RIGHT_BRACE) && !isEOF()) {
    statements.push_back(declaration());
  }

  throwOrAdvanceIfExpect(TokenKind::RIGHT_BRACE, "Expected '}' after block");
  return std::make_unique<BlockStatement>(std::move(statements));
}

std::unique_ptr<Statement> Parser::ifStatement() {
  std::unique_ptr<Statement> thenStatement{};
  std::unique_ptr<Statement> elseStatement{nullptr};
  std::unique_ptr<Expr> condition{};

  throwOrAdvanceIfExpect(TokenKind::LEFT_PAREN, "Expected '(' after if");
  condition = expression();
  throwOrAdvanceIfExpect(TokenKind::RIGHT_PAREN,
                         "Expected ')' after expression");
  thenStatement = statement();

  if (advanceIfExpect(TokenKind::ELSE)) {
    elseStatement = statement();
  }

  return std::make_unique<IfStatement>(
      std::move(condition), std::move(thenStatement), std::move(elseStatement));
}

std::unique_ptr<Statement> Parser::declaration() {
  try {
    if (expect(TokenKind::VAR)) {
      advance();
      return varDeclaration();
    }

    return statement();
  } catch (ParserException &e) {
    synchronize();
    return nullptr;
  }
}

std::unique_ptr<Statement> Parser::varDeclaration() {
  Token name = Token{
      throwOrAdvanceIfExpect(TokenKind::IDENTIFIER, "Expected identifier")};
  std::unique_ptr<Expr> initializer{nullptr};

  if (advanceIfExpect(TokenKind::EQUAL)) {
    initializer = expression();
  }

  throwOrAdvanceIfExpect(TokenKind::SEMICOLON,
                         "Expected ';' after variable declaration");
  return std::make_unique<VarStatement>(name, std::move(initializer));
}

void Parser::synchronize() {
  Token temp = advance();
  while (temp.kind != TokenKind::EOF_TOKEN) {
    switch (temp.kind) {
    case TokenKind::SEMICOLON:
    case TokenKind::CLASS:
    case TokenKind::FUN:
    case TokenKind::VAR:
    case TokenKind::FOR:
    case TokenKind::IF:
    case TokenKind::WHILE:
    case TokenKind::PRINT:
    case TokenKind::RETURN:
      return;
    }
    temp = advance();
  }
}