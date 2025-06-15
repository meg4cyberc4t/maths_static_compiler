#include <fstream>
#include <iostream>

#include <boost/json.hpp>
#include <boost/program_options.hpp>

#include "args.cc"
#include "backend/control_flow_builder.h"
#include "backend/executor.h"
#include "exceptions.h"
#include "frontend/parsing/expression.h"
#include "frontend/parsing/parser.h"
#include "frontend/scanning/lexer.h"
#include "vars.h"

class program
{
public:
  program(int argc, const char* const* argv)
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
      boost::json::object json_debug_obj;

      const auto input_expression = get_input_expression();

      // Syntactic analysis
      auto lexer = frontend::lexer(input_expression);
      auto tokens = lexer.scan_tokens();

      boost::json::array tokens_serialized;
      for (auto token : tokens) {
        tokens_serialized.push_back(token.to_json());
      }
      json_debug_obj["tokens"] = tokens_serialized;

      // Semantic analysis
      auto parser = frontend::parser(tokens);
      auto expr = parser.parse();
      json_debug_obj["syntax_expression_tree"] = expr->to_json();

      // Optimizations
      auto cfb = backend::control_flow_builder(*expr);

      auto exec = backend::executor();
      auto result = exec.execute(cfb.get_data());
      json_debug_obj["cfd"] = cfb.get_data().to_json();
      json_debug_obj["result"] = std::to_string(result);

      // Write additional information to file
      if (options.json_debug_filename.has_value()) {
        auto json_debug_filename = options.json_debug_filename.value();
        std::ofstream json_debug_file(json_debug_filename);
        if (!json_debug_file.is_open()) {
          throw std::invalid_argument("Unable to open file "
                                      + json_debug_filename);
        }
        json_debug_file << json_debug_obj;
        json_debug_file.close();
      }

      std::cout << "Result: " << result << '\n';

      return 0;
    } catch (const std::exception& exception) {
      std::cerr << exception.what();
      return 1;
    }
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

private:
  boost::program_options::options_description description;
  args_options options;
};

int main(int argc, const char* const* argv)
{
  return program(argc, argv).exec();
}
