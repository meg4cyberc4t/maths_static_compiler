#ifndef LEXER_H_
#define LEXER_H_

#include <iostream>
#include <vector>

#include "exceptions.h"
#include "token.h"

namespace frontend
{
class Lexer
{
public:
  Lexer(const std::string& source)
      : source(source)
  {
  }

  std::vector<Token> scan_tokens();

  std::vector<Token> get_tokens() const { return tokens; }

private:
  const std::string& source;

  std::vector<Token> tokens = {};

  std::size_t start_index = 0;
  std::size_t current_index = 0;

  void scan_token();

  void variable();

  void number();

  void add_token(TokenType type);
};
}  // namespace frontend

#endif