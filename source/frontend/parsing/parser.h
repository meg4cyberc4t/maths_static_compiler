#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <memory>
#include <vector>

#include <frontend/exceptions.h>
#include <frontend/parsing/expression.h>
#include <frontend/scanning/token.h>

namespace frontend
{
class parser
{
public:
  parser(std::vector<frontend::token> tokens)
      : tokens(tokens)
  {
  }

  std::unique_ptr<expression> parse() { return term(); }

private:
  std::unique_ptr<expression> term()
  {
    std::unique_ptr<expression> expr = factor();
    while (match({token_type::subtract, token_type::add})) {
      expr = std::make_unique<binary_expression>(
          std::move(expr), previous(), factor());
    };
    return expr;
  }

  std::unique_ptr<expression> factor()
  {
    std::unique_ptr<expression> expr = unary();
    while (match({token_type::delimiter, token_type::multiply})) {
      expr = std::make_unique<binary_expression>(
          std::move(expr), previous(), unary());
    };
    return expr;
  }

  std::unique_ptr<expression> unary()
  {
    std::unique_ptr<expression> expr = primary();
    if (match({token_type::subtract})) {
      expr = std::make_unique<unary_expression>(unary(), previous());
    }
    return expr;
  }

  std::unique_ptr<expression> primary()
  {
    if (match({token_type::number})) {
      return std::make_unique<number_expression>(
          std::stod(previous().get_lexeme()));
    }
    if (match({token_type::variable})) {
      return std::make_unique<variable_expression>(previous());
    }

    if (match({token_type::open_bracket})) {
      std::unique_ptr<expression> expr = parse();
      consume(token_type::close_bracket, "Expect ')' after expression.");
      return std::make_unique<grouping_expression>(std::move(expr));
    }
    throw parse_exception("Expect expression");
  }

  std::vector<frontend::token> tokens;
  std::size_t token_index = 0;

  bool match(std::vector<token_type> types)
  {
    for (auto m_type : types) {
      if (check(m_type)) {
        advance();
        return true;
      }
    }
    return false;
  }

  constexpr bool check(token_type m_type) const
  {
    return !is_at_end() && peek().get_type() == m_type;
  }

  token peek() const { return tokens[token_index]; }

  bool is_at_end() const { return peek().get_type() == eof; }

  token consume(token_type m_type, const std::string& message)
  {
    if (check(m_type)) {
      return advance();
    }
    throw parse_exception(message);
  }

  token advance()
  {
    if (!is_at_end()) {
      token_index += 1;
    }
    return previous();
  }

  token previous() const { return tokens[token_index - 1]; }
};

}  // namespace frontend
#endif
