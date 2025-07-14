#pragma once
#include "models/token.h"
#include <exception>

class RuntimeException : public std::exception {
public:
  Token token;
  std::string message;
  RuntimeException(Token token, std::string message)
      : token{token}, message(std::move(message)) {};
};