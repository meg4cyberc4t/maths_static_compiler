
#include "control_flow_builder.h"

#include "exceptions.h"
#include "frontend/parsing/expression.h"

namespace backend
{

ssa_position control_flow_builder::add_expression(
    const frontend::expression& expr)
{
  if (auto expr_ptr = dynamic_cast<const frontend::binary_expression*>(&expr)) {
    return add_expression(*expr_ptr);
  } else if (auto expr_ptr =
                 dynamic_cast<const frontend::grouping_expression*>(&expr))
  {
    return add_expression(*expr_ptr);
  } else if (auto expr_ptr =
                 dynamic_cast<const frontend::number_expression*>(&expr))
  {
    return add_expression(*expr_ptr);
  } else if (auto expr_ptr =
                 dynamic_cast<const frontend::variable_expression*>(&expr))
  {
    return add_expression(*expr_ptr);
  } else if (auto expr_ptr =
                 dynamic_cast<const frontend::unary_expression*>(&expr))
  {
    return add_expression(*expr_ptr);
  }
  return -1;
};

ssa_position control_flow_builder::add_expression(
    const frontend::number_expression& expr)
{
  data.uses[data.control_flow_index] = {};
  data.defines[data.control_flow_index] = expr.get_value();
  return data.control_flow_index++;
}

ssa_position control_flow_builder::add_expression(
    const frontend::variable_expression& expr)
{
  data.uses[data.control_flow_index] = {};
  data.variables[data.control_flow_index] = expr.get_token().get_lexeme();
  return data.control_flow_index++;
}

ssa_position control_flow_builder::add_expression(
    const frontend::binary_expression& expr)
{
  ssa_position left_position = add_expression(*expr.get_left());
  ssa_position right_position = add_expression(*expr.get_right());

  const std::map<frontend::token_type, expression_op> expression_map = {
      {frontend::token_type::multiply, expression_op::multiply},
      {frontend::token_type::add, expression_op::add},
      {frontend::token_type::subtract, expression_op::subtract},
      {frontend::token_type::delimiter, expression_op::divide},
  };

  auto type = expr.get_token().get_type();
  if (!expression_map.contains(type)) {
    throw control_flow_exception("Not exist expression with type "
                                 + std::to_string(type));
  }
  const auto b_expr =
      expression(left_position, expression_map.at(type), right_position);
  data.expressions.insert(std::make_pair(data.control_flow_index, b_expr));
  data.uses[left_position].insert(data.control_flow_index);
  data.uses[right_position].insert(data.control_flow_index);
  data.uses[data.control_flow_index] = {};
  return data.control_flow_index++;
}

ssa_position control_flow_builder::add_expression(
    const frontend::unary_expression& expr)
{
  ssa_position position = add_expression(*expr.get_expr());

  auto type = expr.get_token().get_type();
  if (type != frontend::token_type::subtract) {
    throw control_flow_exception("Not substract unary op");
  }
  const auto b_expr =
      expression(position, expression_op::subtract, MINUS_ONE_SSA_POSITION);
  data.expressions.insert(std::make_pair(data.control_flow_index, b_expr));
  data.uses[position].insert(data.control_flow_index);
  data.uses[MINUS_ONE_SSA_POSITION].insert(data.control_flow_index);
  data.uses[data.control_flow_index] = {};
  return data.control_flow_index++;
}

ssa_position control_flow_builder::add_expression(
    const frontend::grouping_expression& expr)
{
  return add_expression(*expr.get_expr());
}

void control_flow_builder::optimize()
{
  // Copy propagation
  // Dead code elimination – If exist a block, when one of values is equal zero
  // – remove the branch
  // Constant propagation
  // Defragmentation of indexes
}

void control_flow_builder::execute()
{
  // Request variables && Execute by steps
  std::cout << "Uses:" << std::endl;
  for (const auto& pair : data.uses) {
    std::cout << pair.first << ": {";
    for (auto item : pair.second) {
      std::cout << item << ", ";
    }
    std::cout << "}" << std::endl;
  }
  std::cout << "Expressions:" << std::endl;
  for (const auto& pair : data.expressions) {
    std::cout << pair.first << ": " << pair.second.m_left
              << std::to_string(pair.second.m_operator) << pair.second.m_right
              << std::endl;
  }
  std::cout << "Defines:" << std::endl;
  for (const auto& pair : data.defines) {
    std::cout << pair.first << ": " << pair.second << std::endl;
  }
  std::cout << "Variables:" << std::endl;
  for (const auto& pair : data.variables) {
    std::cout << pair.first << ": " << pair.second << std::endl;
  }
  std::cout << "index: " << std::to_string(data.control_flow_index)
            << std::endl;
}
}  // namespace backend
