#include <iostream>
#include <string>

#include "expression.h"

namespace frontend
{
auto binary_expression::to_string(std::size_t indent) -> std::string
{
  return "binary_expression:\n" + std::string(indent, ' ') + "left – "
      + this->m_left->to_string(indent + 1) + "\n" + std::string(indent, ' ')
      + "token – " + this->m_token.get_lexeme() + "\n"
      + std::string(indent, ' ') + "right – "
      + this->m_right->to_string(indent + 1);

  // binary_expression:
  //  left – ...
  //  token - ..
  //  right - ...
}

auto grouping_expression::to_string(std::size_t indent) -> std::string
{
  return "grouping_expression:\n" + std::string(indent, ' ') + "expr – "
      + this->m_expr->to_string(indent + 1);

  // grouping_expression:
  //  expr – ...
}

auto number_expression::to_string(std::size_t indent) -> std::string
{
  return "number_expression:\n" + std::string(indent, ' ') + "number – "
      + std::to_string(this->m_value);

  // number_expression:
  //  number – ...
}

auto variable_expression::to_string(std::size_t indent) -> std::string
{
  return "variable_expression:\n" + std::string(indent, ' ') + "variable – "
      + this->m_token.get_lexeme();

  // variable_expression:
  //  variable – ...
}

auto unary_expression::to_string(std::size_t indent) -> std::string
{
  return "unary_expression:\n" + std::string(indent, ' ') + "\n"
      + std::string(indent, ' ') + "token – " + this->m_token.get_lexeme()
      + "\n" + std::string(indent, ' ') + "expr – "
      + this->m_expr->to_string(indent + 1);

  // binary_expression:
  //  token – ...
  //  expr - ..
}

}  // namespace frontend
