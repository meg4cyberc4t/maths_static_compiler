#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <vector>

#include "exceptions.h"
#include "token.h"

namespace frontend
{
class lexer
{
public:
  lexer(const std::string& source)
      : m_source(std::move(source))
  {
  }

  std::vector<token> scan_tokens();

  std::vector<token> get_tokens() const { return m_tokens; }

private:
  std::string m_source;

  std::vector<token> m_tokens;

  std::size_t m_start_index = 0;
  std::size_t m_current_index = 0;

  void scan_token();

  void variable();

  void number();

  void add_token(token_type type);
};
}  // namespace frontend

#endif
