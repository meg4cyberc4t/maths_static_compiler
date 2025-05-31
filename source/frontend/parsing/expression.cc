#include <iostream>
#include <string>

#include "expression.h"

namespace frontend
{
auto binary_expression::to_string(std::size_t indent) -> std::string
{
  // binary_expression:
  //  left – ...
  //  token - ..
  //  right - ...
  return "binary_expression:\n" + std::string(indent, ' ') + "left – "
      + this->m_left->to_string(indent + 1) + "\n" + std::string(indent, ' ')
      + "token – " + this->m_token.get_lexeme() + "\n"
      + std::string(indent, ' ') + "right – "
      + this->m_right->to_string(indent + 1);
}

auto grouping_expression::to_string(std::size_t indent) -> std::string
{
  // grouping_expression:
  //  expr – ...
  return "grouping_expression:\n" + std::string(indent, ' ') + "expr – "
      + this->m_expr->to_string(indent + 1);
}

auto number_expression::to_string(std::size_t indent) -> std::string
{
  // number_expression:
  //  number – ...
  return "number_expression:\n" + std::string(indent, ' ') + "number – "
      + std::to_string(this->m_value);
}

auto variable_expression::to_string(std::size_t indent) -> std::string
{
  // variable_expression:
  //  variable – ...
  return "variable_expression:\n" + std::string(indent, ' ') + "variable – "
      + this->m_token.get_lexeme();
}

auto unary_expression::to_string(std::size_t indent) -> std::string
{
  // binary_expression:
  //  token – ...
  //  expr - ..
  return "unary_expression:\n" + std::string(indent, ' ') + "token – "
      + this->m_token.get_lexeme() + "\n" + std::string(indent, ' ') + "expr – "
      + this->m_expr->to_string(indent + 1);
}

}  // namespace frontend
