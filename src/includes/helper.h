#pragma once
#include <string>

namespace error {
void reportError(std::string reason, size_t line);
void reportWarning(std::string reason, size_t line);
}