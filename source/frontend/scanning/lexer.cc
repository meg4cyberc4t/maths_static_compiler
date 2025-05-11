#include <iostream>
#include <map>
#include <vector>

#include "lexer.h"

#include "token.h"

namespace frontend
{

bool constexpr is_number(char symbol)
{
  return '0' <= symbol && symbol <= '9';
}

bool constexpr is_alpha(char symbol)
{
  return ('a' <= symbol && symbol <= 'z') || ('A' <= symbol && symbol <= 'Z');
}

bool constexpr is_whitespace(char symbol)
{
  return symbol == '\n' || symbol == '\r' || symbol == '\t' || symbol == ' ';
}

std::vector<Token> Lexer::scan_tokens()
{
  while (current_index < source.size()) {
    scan_token();
    start_index = current_index;
  }
  return tokens;
}

void Lexer::scan_token()
{
  const char letter = source[current_index++];
  const std::map<char, frontend::TokenType> single_character_operators {
      {'(', TokenType::OPEN_BRACKET},
      {')', TokenType::CLOSE_BRACKET},
      {'*', TokenType::MULTIPLY},
      {'/', TokenType::DELIMITER},
      {'+', TokenType::ADD},
      {'-', TokenType::SUBTRACT},
  };
  if (single_character_operators.contains(letter)) {
    add_token(single_character_operators.at(letter));
    return;
  }
  if (is_whitespace(letter)) {
    return;
  }
  if (is_alpha(letter)) {
    variable();
    return;
  } else if (is_number(letter)) {
    number();
    return;
  } else {
    throw UnknownLiteralException(source, start_index);
  }
}

void Lexer::variable()
{
  std::size_t next_index = current_index;
  while (next_index < source.size()
         && (is_alpha(source[next_index]) || is_number(source[next_index])))
    next_index++;
  current_index = next_index;
  add_token(TokenType::VARIABLE);
}

void Lexer::number()
{
  std::size_t next_index = current_index;
  while (next_index < source.size() && is_number(source[next_index]))
    next_index++;
  if (next_index < source.size() && source[next_index] == '.') {
    next_index++;
    while (next_index < source.size() && is_number(source[next_index]))
      next_index++;
  }
  current_index = next_index;
  add_token(TokenType::NUMBER);
}

void Lexer::add_token(TokenType type)
{
  const std::string lexeme =
      source.substr(start_index, current_index - start_index);
  tokens.emplace_back(Token(type, lexeme, start_index));
}

}  // namespace frontend
