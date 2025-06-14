#include <map>

#include "lexer.h"

#include "token.h"

namespace
{
auto constexpr is_number(char symbol) -> bool
{
  return '0' <= symbol && symbol <= '9';
}

auto constexpr is_alpha(char symbol) -> bool
{
  return ('a' <= symbol && symbol <= 'z') || ('A' <= symbol && symbol <= 'Z');
}

auto constexpr is_whitespace(char symbol) -> bool
{
  return symbol == '\n' || symbol == '\r' || symbol == '\t' || symbol == ' ';
}

}  // namespace

namespace frontend
{

std::vector<token> lexer::scan_tokens()
{
  while (m_current_index < m_source.size()) {
    scan_token();
    m_start_index = m_current_index;
  }
  add_token(eof);
  return m_tokens;
}

void lexer::scan_token()
{
  const char letter = m_source[m_current_index++];
  const std::map<char, frontend::token_type> single_character_operators {
      {'(', token_type::open_bracket},
      {')', token_type::close_bracket},
      {'*', token_type::multiply},
      {'/', token_type::delimiter},
      {'+', token_type::add},
      {'-', token_type::subtract},
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
  }
  if (is_number(letter)) {
    number();
    return;
  }
  throw unknown_literal_error(m_source, m_start_index);
}

void lexer::variable()
{
  std::size_t next_index = m_current_index;
  while (next_index < m_source.size()
         && (is_alpha(m_source[next_index]) || is_number(m_source[next_index])))
  {
    next_index++;
  }
  m_current_index = next_index;
  add_token(token_type::variable);
}

void lexer::number()
{
  std::size_t next_index = m_current_index;
  while (next_index < m_source.size() && is_number(m_source[next_index])) {
    next_index++;
  }
  if (next_index < m_source.size() && m_source[next_index] == '.') {
    next_index++;
    while (next_index < m_source.size() && is_number(m_source[next_index])) {
      next_index++;
    }
  }
  m_current_index = next_index;
  add_token(token_type::number);
}

void lexer::add_token(token_type type)
{
  const std::string m_lexeme =
      m_source.substr(m_start_index, m_current_index - m_start_index);
  m_tokens.emplace_back(type, m_lexeme, m_start_index);
}

}  // namespace frontend
