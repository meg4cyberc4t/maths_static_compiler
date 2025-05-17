#include <iostream>
#include <string>

#include "frontend/parsing/expression.h"
#include "frontend/parsing/parser.h"
#include "frontend/scanning/exceptions.h"
#include "frontend/scanning/lexer.h"

int main()
{
  // cxxopts::Options options("test", "A brief description");

  // options.add_options()("b,bar", "Param bar", cxxopts::value<std::string>())(
  //     "d,debug",
  //     "Enable debugging",
  //     cxxopts::value<bool>()->default_value("false"))(
  //     "f,foo", "Param foo", cxxopts::value<int>()->default_value("10"))(
  //     "h,help", "Print usage");

  // auto result = options.parse(argc, argv);

  // if (result.count("help")) {
  //   std::cout << options.help() << std::endl;
  //   exit(0);
  // }
  // bool debug = result["debug"].as<bool>();
  // std::string bar;
  // if (result.count("bar"))
  //   bar = result["bar"].as<std::string>();
  // int foo = result["foo"].as<int>();

  // return 0;

  // std::cout << "Input the source line:" << std::endl;
  // std::string source;
  // std::getline(std::cin, source);

  const std::string source = "1 + 2 * ( 3 * variable1 * * 1)";
  auto lexer = frontend::Lexer(source);
  try {
    lexer.scan_tokens();
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
    return 65;
  }

  auto tokens = lexer.get_tokens();
  for (auto token : tokens) {
    const std::string message = "(Token: " + std::to_string(token.type)
        + std::string(" ") + token.lexeme + std::string(" ")
        + std::to_string(token.pos) + "), ";
    std::cout << message << std::endl;
  }

  auto parser = frontend::Parser(tokens);
  auto expr = parser.parse();
  std::cout << expr->to_string();

  return 0;
}
