#include <iostream>
#include <string>

#include "frontend/parsing/parser.h"
#include "frontend/scanning/exceptions.h"
#include "frontend/scanning/lexer.h"

int main()
{
  // std::cout << "Input the source line:" << std::endl;
  // std::string source;
  // std::getline(std::cin, source);

  const std::string source = "1 + (2 * 3) + 4";
  auto lexer = frontend::Lexer(source);
  try {
    lexer.scan_tokens();
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
    return 65;
  }

  auto tokens = lexer.get_tokens();
  // for (auto token : tokens) {
  //   const std::string message = "Token: " + std::to_string(token.type)
  //       + std::string(" ") + token.lexeme + std::string(" ")
  //       + std::to_string(token.pos) + '\n';
  //   std::cout << message << std::endl;
  // }

  auto parser = frontend::Parser(tokens);
  auto expr = parser.parse();

  std::cout << expr->to_string() << '\n';

  return 0;
}
