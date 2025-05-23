#include <catch2/catch_test_macros.hpp>

#include "frontend/scanning/lexer.h"
#include "frontend/scanning/token.h"

TEST_CASE("Checking all TokenType's works correctly", "[Lexer]")
{
  using namespace frontend;

  const std::string line = "1 + 11.00 - 1000 / var123 * (5 - 2)";
  Lexer lexer = Lexer(line);
  lexer.scan_tokens();
  auto tokens = lexer.get_tokens();

  const std::vector<Token> exceptedTokens {
      Token(NUMBER, "1", 0),
      Token(ADD, "+", 2),
      Token(NUMBER, "11.00", 4),
      Token(SUBTRACT, "-", 10),
      Token(NUMBER, "1000", 12),
      Token(DELIMITER, "/", 17),
      Token(VARIABLE, "var123", 19),
      Token(MULTIPLY, "*", 26),
      Token(OPEN_BRACKET, "(", 28),
      Token(NUMBER, "5", 29),
      Token(SUBTRACT, "-", 31),
      Token(NUMBER, "2", 33),
      Token(CLOSE_BRACKET, ")", 34),
  };
  REQUIRE(tokens == exceptedTokens);
}

TEST_CASE("Incorrect lexemes cause an error", "[Lexer]")
{
  using namespace frontend;

  const std::string line = "1 + 11.00 - & 1000 / var123 * (5 - 2)";
  Lexer lexer = Lexer(line);
  REQUIRE_THROWS_AS(lexer.scan_tokens(), UnknownLiteralException);
}
