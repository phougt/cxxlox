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

void run(std::string source) {
  Scanner scanner{source};
  std::vector<Token> tokens{scanner.getTokens()};
  Parser parser{tokens};
  auto expr{parser.parse()};

  if (hasError) {
    std::cout << "Parser error occurred. Exiting program.";
    return;
  }
}

void runFile(std::string path) {
  std::fstream file{path, file.in};
  file.exceptions(std::ios_base::badbit);

  if (!file.is_open()) {
    std::cerr << "[Error]: Input path can not be opened.";
  }

  std::stringstream sourceStream{};
  file >> sourceStream.rdbuf();
  run(sourceStream.str());
}

int main() {
  std::chrono::time_point<std::chrono::steady_clock> start{
      std::chrono::steady_clock::now()};

  std::string path{""};
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