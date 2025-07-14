#include "scanner.h"
#include "enums/token_kind.h"
#include "helper.h"
#include "models/token.h"
#include <cctype>
#include <format>
#include <string>

// Default constructor
Scanner::Scanner() {}
Scanner::~Scanner() {}

Scanner::Scanner(std::string &&source) : source(std::move(source)) {
  keywords.insert({"and", TokenKind::AND});
  keywords.insert({"class", TokenKind::CLASS});
  keywords.insert({"else", TokenKind::ELSE});
  keywords.insert({"false", TokenKind::FALSE});
  keywords.insert({"fun", TokenKind::FUN});
  keywords.insert({"for", TokenKind::FOR});
  keywords.insert({"if", TokenKind::IF});
  keywords.insert({"nil", TokenKind::NIL});
  keywords.insert({"or", TokenKind::OR});
  keywords.insert({"print", TokenKind::PRINT});
  keywords.insert({"return", TokenKind::RETURN});
  keywords.insert({"super", TokenKind::SUPER});
  keywords.insert({"this", TokenKind::THIS});
  keywords.insert({"true", TokenKind::TRUE});
  keywords.insert({"var", TokenKind::VAR});
  keywords.insert({"while", TokenKind::WHILE});
}

std::vector<Token> &&Scanner::getTokens() {
  tokens = std::vector<Token>{};

  while (!isEOF()) {
    scanToken();
  }

  tokens.push_back(Token(TokenKind::EOF_TOKEN, "", line));
  return std::move(tokens);
}

void Scanner::scanToken() {
  char temp = peek();
  switch (temp) {
  case '(':
    appendToken(TokenKind::LEFT_PAREN);
    advance();
    break;
  case ')':
    appendToken(TokenKind::RIGHT_PAREN);
    advance();
    break;
  case '{':
    appendToken(TokenKind::LEFT_BRACE);
    advance();
    break;
  case '}':
    appendToken(TokenKind::RIGHT_BRACE);
    advance();
    break;
  case ',':
    appendToken(TokenKind::COMMA);
    advance();
    break;
  case '.':
    appendToken(TokenKind::DOT);
    advance();
    break;
  case '-':
    appendToken(TokenKind::MINUS);
    advance();
    break;
  case '+':
    appendToken(TokenKind::PLUS);
    advance();
    break;
  case ';':
    appendToken(TokenKind::SEMICOLON);
    advance();
    break;
  case '*':
    appendToken(TokenKind::STAR);
    advance();
    break;
  case '/':
    advance();
    if (peek() == '/') {
      while (!isEOF()) {
        if (peek() == '\n') {
          line++;
          advance();
          break;
        }
        advance();
      }
    } else {
      appendToken(TokenKind::SLASH);
    }
    break;
  case '!':
    advance();
    if (peek() == '=') {
      appendToken(TokenKind::BANG_EQUAL);
      advance();
    } else {
      appendToken(TokenKind::BANG);
    }
    break;
  case '=':
    advance();
    if (peek() == '=') {
      appendToken(TokenKind::EQUAL_EQUAL);
      advance();
    } else {
      appendToken(TokenKind::EQUAL);
    }
    break;
  case '>':
    advance();
    if (peek() == '=') {
      appendToken(TokenKind::GREATER_EQUAL);
      advance();
    } else {
      appendToken(TokenKind::GREATER);
    }
    break;
  case '<':
    advance();
    if (peek() == '=') {
      appendToken(TokenKind::LESS_EQUAL);
      advance();
    } else {
      appendToken(TokenKind::LESS);
    }
    break;
  case '"':
    scanStringLiteral();
    break;
  case ' ':
  case '\t':
    advance();
    break;
  case '\n':
  case '\r':
    advance();
    line++;
    break;
  default:
    if (std::isdigit(temp)) {
      scanNumber();
    } else if (std::isalpha(temp) || std::isdigit(temp) || temp == '_') {
      scanIdentifierAndKeyword();
    } else {
      error::reportError(std::format("Illegal char ({}) found", temp), line);
      advance();
    }
    break;
  }
}

void Scanner::scanNumber() {
  std::string number{};

  while (std::isdigit(peek())) {
    number.push_back(advance());
  }

  if (peek() == '.' && std::isdigit(peek(2))) {
    number.push_back(advance());
    while (std::isdigit(peek())) {
      number.push_back(advance());
    }
  }

  appendToken(TokenKind::NUMBER, std::stod(number));
}

void Scanner::scanStringLiteral() {
  std::string literal{};

  if (peek() == '"') {
    advance();

    while (peek() != '"' && !isEOF()) {
      if (peek() == '\n') {
        line++;
      }
      literal.push_back(advance());
    }

    if (peek() == '"') {
      advance();
    } else {
      error::reportError("Unterminated string literal", line);
      return;
    }
  }

  appendToken(TokenKind::STRING, literal);
}

void Scanner::scanIdentifierAndKeyword() {
  std::string identifier{};

  while (std::isalpha(peek()) || std::isdigit(peek()) || peek() == '_') {
    identifier.push_back(advance());
  }

  if (keywords.contains(identifier)) {
    appendToken(keywords.at(identifier));
    return;
  }

  appendToken(TokenKind::IDENTIFIER, identifier);
}

bool Scanner::isEOF() { return current >= source.length(); }

char Scanner::peek(int count) {
  if (current + count - 1 >= source.length() || count < 0) {
    return -1;
  }
  return source.at(current + count - 1);
}

char Scanner::advance() { return source.at(current++); }

void Scanner::appendToken(TokenKind kind, double literal) {
  tokens.push_back(Token(kind, literal, line));
}

void Scanner::appendToken(TokenKind kind, std::string literal) {
  tokens.push_back(Token(kind, literal, line));
}

void Scanner::appendToken(TokenKind kind) {
  tokens.push_back(Token(kind, "", line));
}
