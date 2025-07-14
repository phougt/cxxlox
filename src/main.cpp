#include "includes/interpreter.h"
#include "parser.h"
#include "scanner.h"
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

void runREPL() {
  std::string line;
  Interpreter interpreter;

  while (true) {
    std::cout << "> ";
    std::getline(std::cin, line);

    if (line.empty())
      continue;

    if (line == "exit" || line == "quit")
      break;

    run(std::move(line));
    hasError = false;
    hasRuntimeError = false;
  }
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    runREPL();
  } else if (argc == 2) {
    try {
      std::string path{argv[1]};
      runFile(path);
    } catch (std::exception &e) {
      std::cerr << "[Error]: " << e.what() << std::endl;
      return 1;
    }
  }

  return 0;
}