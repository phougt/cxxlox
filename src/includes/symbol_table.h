#pragma once
#include "common.h"
#include "models/token.h"
#include <map>
#include <memory>

class SymbolTable {
public:
  SymbolTable(std::unique_ptr<SymbolTable> &&parent)
      : parent{std::move(parent)}, table{} {};
  SymbolTable() : table{} {};
  ~SymbolTable() = default;
  std::unique_ptr<SymbolTable> &&getParent();
  LoxValue getValue(Token variable);
  void define(Token variable, LoxValue value);
  void assign(Token variable, LoxValue value);

private:
  std::unique_ptr<SymbolTable> parent;
  std::map<std::string, LoxValue> table;
};