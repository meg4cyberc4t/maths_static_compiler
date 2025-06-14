#include <iostream>
#include <string>

#include "expression.h"

namespace frontend
{

boost::json::object binary_expression::to_json() const
{
  boost::json::object obj;
  obj["type"] = "binary";
  obj["left"] = m_left->to_json();
  obj["token_type"] = token_type_to_string(m_token.get_type());
  obj["right"] = m_right->to_json();
  return obj;
}

boost::json::object grouping_expression::to_json() const
{
  boost::json::object obj;
  obj["type"] = "grouping";
  obj["expr"] = m_expr->to_json();
  return obj;
}

boost::json::object number_expression::to_json() const
{
  boost::json::object obj;
  obj["type"] = "number";
  obj["value"] = std::to_string(m_value);
  return obj;
}

boost::json::object variable_expression::to_json() const
{
  boost::json::object obj;
  obj["type"] = "variable";
  obj["lexeme"] = m_token.get_lexeme();
  return obj;
}

boost::json::object unary_expression::to_json() const
{
  boost::json::object obj;
  obj["type"] = "unary";
  obj["token_type"] = token_type_to_string(m_token.get_type());
  obj["expr"] = m_expr->to_json();
  return obj;
}

}  // namespace frontend
