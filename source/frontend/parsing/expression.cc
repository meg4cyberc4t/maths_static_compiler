#include <iostream>

#include "expression.h"

namespace frontend
{
std::string BinaryExpression::to_string(int indent)
{
  return "BinaryExpression:\n" + std::string(indent, ' ') + "left – "
      + this->left->to_string(indent + 1) + "\n" + std::string(indent, ' ')
      + "token – " + this->token.lexeme + "\n" + std::string(indent, ' ')
      + "right – " + this->right->to_string(indent + 1);

  // BinaryExpression:
  //  left – ...
  //  token - ..
  //  right - ...
};

std::string GroupingExpression::to_string(int indent)
{
  return "GroupingExpression:\n" + std::string(indent, ' ') + "expr – "
      + this->expr->to_string(indent + 1);

  // GroupingExpression:
  //  expr – ...
};

std::string NumberExpression::to_string(int indent)
{
  return "NumberExpression:\n" + std::string(indent, ' ') + "number – "
      + std::to_string(this->value);

  // NumberExpression:
  //  number – ...
};

std::string VariableExpression::to_string(int indent)
{
  return "VariableExpression:\n" + std::string(indent, ' ') + "variable – "
      + this->token.lexeme;

  // VariableExpression:
  //  variable – ...
};

std::string UnaryExpression::to_string(int indent)
{
  return "UnaryExpression:\n" + std::string(indent, ' ') + "\n"
      + std::string(indent, ' ') + "token – " + this->token.lexeme + "\n"
      + std::string(indent, ' ') + "expr – "
      + this->expr->to_string(indent + 1);

  // BinaryExpression:
  //  token – ...
  //  expr - ..
};

}  // namespace frontend