#ifndef TOKEN_H_
#define TOKEN_H_

#include <iostream>

namespace frontend
{
enum TokenType : uint
{
  OPEN_BRACKET,  // (
  CLOSE_BRACKET,  // )

  MULTIPLY,  // *
  PLUS,  // +
  MINUS,  // -
  DELIMETER,  // /

  NUMBER,  // 0-9* | 0-9*.0-9*
  VARIABLE,  // a-z*0-9*
};

struct Token
{
public:
  Token(const TokenType type, const std::string lexeme, const std::size_t pos)
      : lexeme(lexeme)
      , type(type)
      , pos(pos)
  {
  }

  bool operator==(const Token& other) const
  {
    return type == other.type && lexeme == other.lexeme && pos == other.pos;
  }

  const std::string lexeme;
  const TokenType type;
  const std::size_t pos;
};

}  // namespace frontend

#endif