#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>

#include <frontend/parsing/exceptions.h>
#include <frontend/parsing/expression.h>
#include <frontend/scanning/token.h>

namespace frontend
{
class Parser
{
public:
  Parser(std::vector<frontend::Token> tokens)
      : tokens(tokens)
  {
  }

  std::unique_ptr<Expression> parse() { return term(); }

private:
  std::unique_ptr<Expression> term()
  {
    std::unique_ptr<Expression> expr = factor();
    while (match({TokenType::SUBTRACT, TokenType::ADD})) {
      expr = std::make_unique<BinaryExpression>(
          std::move(expr), previous(), std::move(factor()));
    };
    return expr;
  }

  std::unique_ptr<Expression> factor()
  {
    std::unique_ptr<Expression> expr = unary();
    while (match({TokenType::DELIMITER, TokenType::MULTIPLY})) {
      expr = std::make_unique<BinaryExpression>(
          std::move(expr), previous(), std::move(unary()));
    };
    return expr;
  }

  std::unique_ptr<Expression> unary()
  {
    std::unique_ptr<Expression> expr = primary();
    if (match({TokenType::SUBTRACT})) {
      expr = std::make_unique<UnaryExpression>(std::move(unary()), previous());
    }
    return expr;
  }

  std::unique_ptr<Expression> primary()
  {
    if (match({TokenType::NUMBER})) {
      return std::make_unique<NumberExpression>(std::stof(previous().lexeme));
    }
    if (match({TokenType::VARIABLE})) {
      return std::make_unique<VariableExpression>(previous());
    }

    if (match({TokenType::OPEN_BRACKET})) {
      std::unique_ptr<Expression> expr = parse();
      consume(TokenType::CLOSE_BRACKET, "Expect ')' after expression.");
      return std::make_unique<GroupingExpression>(std::move(expr));
    }
    throw ParseException("Expect expression");
  }

  std::vector<frontend::Token> tokens;
  size_t token_index = 0;

  bool match(std::vector<TokenType> types)
  {
    for (auto type : types) {
      if (check(type)) {
        advance();
        return true;
      }
    }
    return false;
  }

  bool constexpr check(TokenType type)
  {
    return !is_at_end() && peek().type == type;
  }

  Token peek() const { return tokens[token_index]; }

  bool is_at_end() const { return peek().type == _EOF; }

  Token consume(TokenType type, const std::string& message)
  {
    if (check(type))
      return advance();
    throw ParseException(message);
  }

  Token advance()
  {
    if (!is_at_end()) {
      token_index++;
    }
    return previous();
  }
  Token previous() const { return tokens[token_index - 1]; }
};

}  // namespace frontend
#endif
