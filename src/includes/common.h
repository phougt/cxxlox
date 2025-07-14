#pragma once
#include <string>
#include <variant>

extern bool hasError;
extern bool hasRuntimeError;

using LoxValue = std::variant<std::monostate, std::string, double, bool>;