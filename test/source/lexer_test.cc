#include "frontend/scanning/lexer.h"

#include <catch2/catch_test_macros.hpp>

#include "frontend/scanning/token.h"

TEST_CASE("Checking all token_type's works correctly", "[lexer]")
{
  std::string line = "1 + 11.00 - 1000 / var123 * (5 - 2)";
  auto lexer = frontend::lexer(line);
  lexer.scan_tokens();
  auto tokens = lexer.get_tokens();

  using namespace frontend;

  const std::vector<token> excepted_tokens {
      token(number, "1", 0),
      token(add, "+", 2),
      token(number, "11.00", 4),
      token(subtract, "-", 10),
      token(number, "1000", 12),
      token(delimiter, "/", 17),
      token(variable, "var123", 19),
      token(multiply, "*", 26),
      token(open_bracket, "(", 28),
      token(number, "5", 29),
      token(subtract, "-", 31),
      token(number, "2", 33),
      token(close_bracket, ")", 34),
      token(eof, "", 35),
  };
  REQUIRE(tokens == excepted_tokens);
}

TEST_CASE("Incorrect lexemes cause an error", "[lexer]")
{
  std::string line = "1 + 11.00 - & 1000 / var123 * (5 - 2)";
  auto lexer = frontend::lexer(line);
  REQUIRE_THROWS_AS(lexer.scan_tokens(), unknown_literal_exception);
}
