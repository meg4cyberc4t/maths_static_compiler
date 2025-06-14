#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

#include <boost/json.hpp>

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

static const char* token_type_to_string(token_type type)
{
  static const std::array<const char*, 9> mapper = {"open_bracket",
                                                    "close_bracket",
                                                    "multiply",
                                                    "add",
                                                    "subtract",
                                                    "delimiter",
                                                    "number",
                                                    "variable",
                                                    "eof"};
  return mapper[static_cast<size_t>(type)];
}

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

  boost::json::object to_json() const
  {
    boost::json::object obj;
    obj["type"] = token_type_to_string(m_type);
    obj["lexeme"] = m_lexeme;
    obj["pos"] = m_pos;
    return obj;
  }

private:
  std::string m_lexeme;
  token_type m_type;
  std::size_t m_pos;
};

}  // namespace frontend

#endif
