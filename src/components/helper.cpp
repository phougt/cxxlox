#include "helper.h"
#include "common.h"
#include <format>
#include <iostream>

namespace error {
void reportError(std::string reason, size_t line) {
  hasError = true;
  std::cerr << std::format("[Error]: {} on line {}.", reason, line);
}

void reportWarning(std::string reason, size_t line) {
  std::cerr << std::format("[Warning]: {} on line {}.", reason, line);
}
}