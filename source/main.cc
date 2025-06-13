#include <fstream>
#include <iostream>

#include <boost/program_options.hpp>

#include "args.cc"
#include "backend/control_flow_builder.h"
#include "exceptions.h"
#include "frontend/parsing/expression.h"
#include "frontend/parsing/parser.h"
#include "frontend/scanning/lexer.h"
#include "vars.h"

class Program
{
public:
  Program(int argc, const char* const* argv)
      : description {generate_description()}
      , options {parse_options(argc, argv, description)}
  {
  }

  int exec()
  {
    if (options.help) {
      return help();
    }
    if (options.version) {
      return version();
    }

    try {
      const auto input_expression = get_input_expression();

      // Syntactic analysis
      auto lexer = frontend::lexer(input_expression);
      auto tokens = lexer.scan_tokens();

      // Print tokens
      if (options.print_tokens || options.print_tokens_filename.has_value()) {
        print_tokens(tokens);
      }

      // Semantic analysis
      auto parser = frontend::parser(tokens);
      auto expr = parser.parse();

      // Print AST
      if (options.print_syntax_tree
          || options.print_syntax_tree_filename.has_value())
      {
        print_syntax_tree(expr.get());
      }

      auto cfb = backend::control_flow_builder();

      cfb.add_expression(*expr);

      cfb.optimize();

      cfb.execute();

    } catch (const std::exception& exception) {
      std::cerr << exception.what();
      return 1;
    }
    return 0;
  }

private:
  // Display help message
  // Triggered by flag --help
  inline constexpr int help() const
  {
    std::cout << description << "\n";
    return 0;
  }

  // Display information about the software version
  // Triggered by flag --version
  inline int version() const
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

  // Entering an expression
  // The [--input_line,-i] flags or requested from the user (std::cin)
  std::string get_input_expression() const
  {
    if (options.input_line.has_value()) {
      return options.input_line.value();
    }
    std::string input_line;
    std::cout << "Input the source line:" << std::endl;
    std::getline(std::cin, input_line);
    return input_line;
  }

  // Output of tokens
  void print_tokens(std::vector<frontend::token> tokens) const
  {
    std::stringstream token_string_stream;
    for (auto token : tokens) {
      token_string_stream << token.to_string() + '\n';
    }

    if (options.print_tokens) {
      std::cout << token_string_stream.str();
    }

    if (options.print_tokens_filename.has_value()) {
      auto tokens_filename = options.print_tokens_filename.value();
      std::ofstream tokens_file(tokens_filename);
      if (!tokens_file.is_open()) {
        throw std::make_exception_ptr("Unable to open file " + tokens_filename);
      }
      tokens_file << token_string_stream.str();
      tokens_file.close();
    }
  }

  // Output of abstract syntax tree
  void print_syntax_tree(frontend::expression* expr) const
  {
    auto syntax_tree = expr->to_string(0);
    if (options.print_syntax_tree) {
      std::cout << syntax_tree;
    }
    if (options.print_syntax_tree_filename.has_value()) {
      auto syntax_tree_filename = options.print_syntax_tree_filename.value();
      std::ofstream syntax_tree_file(syntax_tree_filename);
      if (!syntax_tree_file.is_open()) {
        throw std::make_exception_ptr("Unable to open file "
                                      + syntax_tree_filename);
      }
      syntax_tree_file << syntax_tree;
      syntax_tree_file.close();
    }
  }

private:
  boost::program_options::options_description description;
  ArgsOptions options;
};

int main(int argc, const char* const* argv)
{
  return Program(argc, argv).exec();
}
