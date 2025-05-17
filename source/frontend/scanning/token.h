#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>

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

  auto operator==(const token& other) const -> bool
  {
    return m_type == other.m_type && m_lexeme == other.m_lexeme
        && m_pos == other.m_pos;
  }

  auto get_lexeme() const -> std::string { return m_lexeme; }

  auto constexpr get_type() const -> token_type { return m_type; }

  auto constexpr get_pos() const -> std::size_t { return m_pos; }

  auto to_string() const -> std::string
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
