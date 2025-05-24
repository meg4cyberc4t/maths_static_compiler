#include <fstream>
#include <iostream>

#include <boost/program_options.hpp>

#include "args.cc"
#include "frontend/exceptions.h"
#include "frontend/parsing/expression.h"
#include "frontend/parsing/parser.h"
#include "frontend/scanning/lexer.h"

std::string _print_tokens(std::vector<frontend::token> tokens)
{
  std::stringstream ss;
  for (auto token : tokens) {
    ss << token.to_string() + '\n';
  }
  return ss.str();
}

int exec(int argc, const char* const* argv)
{
  auto options = parse_options(argc, argv);

  // Display help message
  if (options.help) {
    std::cout << options.description << "\n";
    return 0;
  }
  // Display information about the software version
  else if (options.version)
  {
    static const std::string banner =
        "     ▗ ▌       ▗   ▗ ▘             ▘▜     \n"
        "▛▛▌▀▌▜▘▛▌▛▘  ▛▘▜▘▀▌▜▘▌▛▘  ▛▘▛▌▛▛▌▛▌▌▐ █▌▛▘\n"
        "▌▌▌█▌▐▖▌▌▄▌▄▖▄▌▐▖█▌▐▖▌▙▖▄▖▙▖▙▌▌▌▌▙▌▌▐▖▙▖▌ \n"
        "                                 ▌        \n";
    std::cout << banner << "Version " << PROJECT_VERSION_STRING << "\n\n"
              << "Source code of the program: " << PROJECT_HOMEPAGE_URL << "\n";
    return 0;
  }

  // expression input. Taken from the [--input_line,-i] flags or requested from
  // the user
  std::string input_line;
  if (!options.input_line.has_value()) {
    std::cout << "Input the source line:" << std::endl;
    std::getline(std::cin, input_line);
  } else {
    input_line = options.input_line.value();
  }

  // Syntactic analysis
  auto lexer = frontend::lexer(input_line);
  auto tokens = lexer.scan_tokens();

  // Output of parsing tokens
  auto print_tokens = _print_tokens(tokens);
  if (options.print_tokens) {
    std::cout << print_tokens;
  }
  if (options.print_tokens_filename.has_value()) {
    auto tokens_filename = options.print_tokens_filename.value();
    std::ofstream tokens_file(tokens_filename);
    if (!tokens_file.is_open()) {
      std::cerr << "Unable to open file " + tokens_filename;
      return 1;
    }

    tokens_file << print_tokens;
    tokens_file.close();
  }

  // Semantic analysis
  auto parser = frontend::parser(tokens);
  auto expr = parser.parse();

  // Output of syntax tree
  auto print_syntax_tree = expr->to_string(0);
  if (options.print_syntax_tree) {
    std::cout << print_syntax_tree;
  }
  if (options.print_syntax_tree_filename.has_value()) {
    auto syntax_tree_filename = options.print_syntax_tree_filename.value();
    std::ofstream syntax_tree_file(syntax_tree_filename);
    if (!syntax_tree_file.is_open()) {
      std::cerr << "Unable to open file " + syntax_tree_filename;
      return 1;
    }
    syntax_tree_file << print_syntax_tree;
    syntax_tree_file.close();
  }
  return 0;
}

int main(int argc, const char* const* argv)
{
  try {
    return exec(argc, argv);
  } catch (const std::exception& exception) {
    std::cerr << exception.what();
    return 1;
  }
}
