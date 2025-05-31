#include "frontend/parsing/parser.h"

#include <catch2/catch_test_macros.hpp>

#include "frontend/parsing/expression.h"

TEST_CASE("Checking the operation of factor expressions", "[parser]")
{
  using namespace frontend;
  const std::vector<token> tokens {
      token(number, "1", 0),
      token(add, "+", 1),
      token(number, "2", 2),
      token(eof, "", 3),
  };
  auto parser = frontend::parser(tokens);
  std::unique_ptr<expression> expr_ptr = parser.parse();
  auto expected_expression = binary_expression(
      std::make_unique<number_expression>(number_expression(1.000000)),
      token(token_type::add, "+", 1),
      std::make_unique<number_expression>(number_expression(2.000000)));
  INFO(expected_expression.to_string(0));
  INFO(expr_ptr->to_string(0));
  REQUIRE(expected_expression == *expr_ptr);
}
