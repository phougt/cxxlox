#include "includes/interpreter.h"
#include "parser.h"
#include "scanner.h"
#include <chrono>
#include <exception>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool hasError = false;
bool hasRuntimeError = false;

void run(std::string &&source) {
  Scanner scanner{std::move(source)};
  std::vector<Token> tokens{scanner.getTokens()};

  if (hasError)
    return;

  Parser parser{std::move(tokens)};
  auto expr{parser.parse()};

  if (hasError)
    return;

  Interpreter interpreter;
  interpreter.interpret(*expr.get());
}

void runFile(std::string path) {
  std::fstream file{path, file.in};
  file.exceptions(std::ios_base::badbit);

  if (!file.is_open()) {
    std::cerr << "[Error]: Input path can not be opened.";
    return;
  }

  std::stringstream sourceStream{};
  file >> sourceStream.rdbuf();
  run(sourceStream.str());

  if (hasError)
    exit(10);
  if (hasRuntimeError)
    exit(20);
}

int main() {
  std::chrono::time_point<std::chrono::steady_clock> start{
      std::chrono::steady_clock::now()};

  std::string path{"D:\\school\\coding\\self_learning\\test_files\\test.txt"};
  try {
    runFile(path);
  } catch (std::exception e) {
    std::cout << e.what();
  }

  std::chrono::time_point<std::chrono::steady_clock> end{
      std::chrono::steady_clock::now()};
  std::cout << std::chrono::duration<double>{end - start};
  return 0;
}