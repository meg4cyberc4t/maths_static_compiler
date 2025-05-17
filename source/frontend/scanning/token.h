#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

namespace frontend
{
enum token_type : unsigned char
{
  open_bracket,  // (
  close_bracket,  // )

  multiply,  // *
  add,  // +
  subtract,  // -
  delimiter,  // /

  number,  // 0-9* | 0-9*.0-9*
  variable,  // a-z*0-9*

  eof  // End of expression / input
};

struct token
{
  token(const token_type type, std::string lexeme, const std::size_t pos)
      : lexeme(std::move(lexeme))
      , type(type)
      , pos(pos)
  {
  }

  bool operator==(const token& other) const
  {
    return type == other.type && lexeme == other.lexeme && pos == other.pos;
  }

  std::string get_lexeme() const { return lexeme; }

  token_type constexpr get_type() const { return type; }

  std::size_t constexpr get_pos() const { return pos; }

  std::string to_string() const
  {
    return "Token { type: " + std::to_string(type) + ", lexeme: " + lexeme
        + ", position: " + std::to_string(pos) + " }";
  }

private:
  std::string lexeme;
  token_type type;
  std::size_t pos;
};

}  // namespace frontend

#endif
