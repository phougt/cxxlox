#pragma once
#include "enums/token_kind.h"
#include "models/token.h"
#include <cstddef>
#include <map>
#include <string>
#include <vector>

class Scanner {
public:
  Scanner();
  Scanner(std::string &source);
  ~Scanner();
  std::vector<Token> getTokens();
  std::string source;

private:
  bool isEOF();
  char peek(int count = 1);
  char advance();
  void scanNumber();
  void scanIdentifierAndKeyword();
  void scanStringLiteral();
  void scanToken();
  void appendToken(TokenKind, double);
  void appendToken(TokenKind, std::string);
  void appendToken(TokenKind);

  size_t current{0};
  size_t line{1};
  std::vector<Token> tokens;
  std::map<std::string, TokenKind> keywords;
};