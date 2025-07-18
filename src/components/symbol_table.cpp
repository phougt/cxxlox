#include "symbol_table.h"
#include "common.h"
#include "exceptions/runtime_exception.h"
#include <format>
#include <memory>

void SymbolTable::define(Token variable, LoxValue value) {
  table.insert_or_assign(std::get<std::string>(variable.literal), value);
}

void SymbolTable::assign(Token variable, LoxValue value) {
  std::string varName = std::get<std::string>(variable.literal);

  if (table.contains(varName)) {
    table.insert_or_assign(varName, value);
    return;
  }

  if (parent != nullptr) {
    parent->assign(variable, value);
    return;
  }

  throw RuntimeException(variable,
                         std::format("Undefined variable '{}'", varName));
}

LoxValue SymbolTable::getValue(Token variable) {
  std::string varName = std::get<std::string>(variable.literal);

  if (table.contains(varName)) {
    return table.at(varName);
  }

  if (parent != nullptr) {
    return parent->getValue(variable);
  }

  throw RuntimeException(variable,
                         std::format("Undefined variable '{}'", varName));
}

std::unique_ptr<SymbolTable> &&SymbolTable::getParent() {
  return std::move(parent);
}