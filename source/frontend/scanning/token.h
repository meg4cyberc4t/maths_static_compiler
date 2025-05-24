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
      : m_lexeme(std::move(lexeme))
      , m_type(type)
      , m_pos(pos)
  {
  }

  bool operator==(const token& other) const
  {
    return m_type == other.m_type && m_lexeme == other.m_lexeme
        && m_pos == other.m_pos;
  }

  std::string get_lexeme() const { return m_lexeme; }

  token_type constexpr get_type() const { return m_type; }

  std::size_t constexpr get_pos() const { return m_pos; }

  std::string to_string() const
  {
    return "Token { type: " + std::to_string(m_type) + ", lexeme: " + m_lexeme
        + ", position: " + std::to_string(m_pos) + " }";
  }

private:
  std::string m_lexeme;
  token_type m_type;
  std::size_t m_pos;
};

}  // namespace frontend

#endif
