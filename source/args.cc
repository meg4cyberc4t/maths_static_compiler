#include <iostream>
#include <optional>

#include <boost/program_options.hpp>

#include "exceptions.h"
#include "frontend/parsing/expression.h"
#include "frontend/parsing/parser.h"
#include "frontend/scanning/lexer.h"

namespace po = boost::program_options;

struct ArgsOptions
{
  bool help;
  bool version;

  std::optional<std::string> input_line;
  std::optional<std::string> json_debug_filename;
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
      "Entering a mathematical expression in a line, e.g. (1 + 2) * 3");
  po::options_description debug_desc("Debug options");
  debug_desc.add_options()(
      "json-debug-file,o",
      po::value<std::string>(),
      "Enter the name of the file to output, e.g. filename.txt");
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
      .json_debug_filename = vm.count("json-debug-file")
          ? vm.at("json-debug-file").as<std::string>()
          : std::optional<std::string>(std::nullopt),
  };
}
