#include <iostream>
#include <optional>

#include <boost/program_options.hpp>

#include "frontend/exceptions.h"
#include "frontend/parsing/expression.h"
#include "frontend/parsing/parser.h"
#include "frontend/scanning/lexer.h"

namespace po = boost::program_options;

struct ArgsOptions
{
  bool help;
  bool version;

  std::optional<std::string> input_line;
  // std::optional<std::string> output_file;

  bool print_tokens;
  std::optional<std::string> print_tokens_filename;
  bool print_syntax_tree;
  std::optional<std::string> print_syntax_tree_filename;
};

static inline po::options_description generate_description()
{
  po::options_description
      desc(
          "A command-line utility for optimizing simple mathematical expressions"
          "\n\n"
          "Allowed options");
  desc.add_options()("help,h", "Produce help message")(
      "version,v", "Produce version string")(
      "input-line,i",
      po::value<std::string>(),
      "Entering a mathematical expression in a line, e.g. (1 + 2) * 3")
      // ("output-file,o", po::value<std::string>(), "Enter the name of the file
      // to output, e.g. filename.txt")
      ;

  po::options_description debug_desc("Debug options");
  debug_desc.add_options()(
      "print-tokens",
      "Extract tokens from an expression after lexical analysis")(
      "print-tokens-filename",
      po::value<std::string>(),
      "Extract tokens from an expression after lexical analysis to the file")(
      "print-syntax-tree",
      "Extract AST from list of tokens after syntax analysis")(
      "print-syntax-tree-filename",
      po::value<std::string>(),
      "Extract AST from list of tokens after syntax analysis to the file");
  desc.add(debug_desc);
  return desc;
}

static inline ArgsOptions parse_options(int argc,
                                        const char* const* argv,
                                        po::options_description desc)
{
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);
  return ArgsOptions {
      .help = vm.count("help") > 0,
      .version = vm.count("version") > 0,
      .input_line = vm.count("input-line")
          ? vm.at("input-line").as<std::string>()
          : std::optional<std::string>(std::nullopt),
      //   .output_file = vm.count("output-file") ?
      //   vm.at("output-line").as<std::string>() :
      //   std::optional<std::string>(std::nullopt),
      .print_tokens = vm.count("print-tokens") > 0,
      .print_tokens_filename = vm.count("print-tokens-filename")
          ? vm.at("print-tokens-filename").as<std::string>()
          : std::optional<std::string>(std::nullopt),
      .print_syntax_tree = vm.count("print-syntax-tree") > 0,
      .print_syntax_tree_filename = vm.count("print-syntax-tree-filename")
          ? vm.at("print-syntax-tree-filename").as<std::string>()
          : std::optional<std::string>(std::nullopt),
  };
}
