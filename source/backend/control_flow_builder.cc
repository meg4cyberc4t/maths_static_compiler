
#include "control_flow_builder.h"

#include "exceptions.h"
#include "frontend/parsing/expression.h"

namespace backend
{

ssa_position control_flow_builder::add_expression(
    const frontend::expression& expr)
{
  ssa_position position;
  if (auto expr_ptr = dynamic_cast<const frontend::binary_expression*>(&expr)) {
    position = add_expression(*expr_ptr);
  } else if (auto expr_ptr =
                 dynamic_cast<const frontend::grouping_expression*>(&expr))
  {
    position = add_expression(*expr_ptr);
  } else if (auto expr_ptr =
                 dynamic_cast<const frontend::number_expression*>(&expr))
  {
    position = add_expression(*expr_ptr);
  } else if (auto expr_ptr =
                 dynamic_cast<const frontend::variable_expression*>(&expr))
  {
    position = add_expression(*expr_ptr);
  } else if (auto expr_ptr =
                 dynamic_cast<const frontend::unary_expression*>(&expr))
  {
    position = add_expression(*expr_ptr);
  } else {
    position = -1;  // unreachable;
  }
  data.out_index = data.control_flow_index - 1;
  return position;
};

ssa_position control_flow_builder::add_expression(
    const frontend::number_expression& expr)
{
  auto value = expr.get_value();
  data.uses[data.control_flow_index] = {};
  auto it =
      std::find_if(data.defines.begin(),
                   data.defines.end(),
                   [&](const auto& pair) { return pair.second == value; });
  if (it != data.defines.end()) {
    return it->first;
  } else {
    data.defines[data.control_flow_index] = value;
    return data.control_flow_index++;
  }
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
      expression(position, expression_op::multiply, MINUS_ONE_SSA_POSITION);
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

void control_flow_builder::replace_position(ssa_position old_position,
                                            ssa_position new_position)
{
  for (auto dependency_pos : data.uses.at(old_position)) {
    data.expressions.at(dependency_pos)
        .replace_position(old_position, new_position);
  }
  data.uses.erase(old_position);
  if (data.out_index == old_position) {
    data.out_index = new_position;
  }
}

void control_flow_builder::copy_propagation()
{
  std::map<backend::expression, ssa_position> expressions_using = {};
  for (auto pair : data.expressions) {
    if (!expressions_using.contains(pair.second)) {
      expressions_using[pair.second] = pair.first;
    } else {
      replace_position(pair.first, expressions_using[pair.second]);
    }
  }
}

void control_flow_builder::algebraic_simplification()
{
  for (auto& [pos, expr] : data.expressions) {
    if ((expr.m_operator == expression_op::multiply
         && expr.is_one_of_positions_equal(ZERO_SSA_POSITION))  // x * 0 = 0
        || (expr.m_operator == expression_op::subtract
            && expr.m_left == expr.m_right))  // x - x = 0
    {
      replace_position(pos, ZERO_SSA_POSITION);
    } else if ((expr.m_operator == expression_op::multiply
                && expr.is_one_of_positions_equal(
                    ONE_SSA_POSITION))  // x * 1 = x
               || (expr.m_operator == expression_op::divide
                   && expr.is_one_of_positions_equal(
                       ONE_SSA_POSITION))  // x / 1 = x
               || (expr.m_operator == expression_op::add
                   && expr.is_one_of_positions_equal(
                       ZERO_SSA_POSITION))  // x + 0 = x
               || (expr.m_operator == expression_op::subtract
                   && expr.is_one_of_positions_equal(
                       ZERO_SSA_POSITION)))  // x - 0 = x
    {
      auto x_position =
          (expr.m_left == 0 || expr.m_left == 1) ? expr.m_right : expr.m_left;
      replace_position(pos, x_position);
    }
  }
}

void control_flow_builder::dead_code_elimination()
{
  std::set<ssa_position> marked_positions = {};
  std::vector<ssa_position> worked_positions = {
      data.out_index,
  };
  while (!worked_positions.empty()) {
    auto pos = worked_positions.back();
    worked_positions.pop_back();
    marked_positions.insert(pos);
    if (data.expressions.contains(pos)) {
      auto expr = data.expressions.at(pos);
      worked_positions.push_back(expr.m_left);
      worked_positions.push_back(expr.m_right);
    }
  }
  for (int i = 0; i < data.control_flow_index; i++) {
    if (marked_positions.contains(i)) {
      continue;
    }
    if (data.expressions.contains(i)) {
      auto expr = data.expressions.at(i);
      data.uses[expr.m_left].erase(i);
      data.uses[expr.m_right].erase(i);
      data.expressions.erase(i);
    }
    data.variables.erase(i);
    data.defines.erase(i);
    data.uses.erase(i);
  }
}

void control_flow_builder::execute()
{
  for (const auto& pair : data.defines) {
    std::cout << "%" << pair.first << " = " << pair.second << std::endl;
  }
  for (const auto& pair : data.variables) {
    std::cout << "%" << pair.first << " = " << pair.second << std::endl;
  }
  for (const auto& pair : data.expressions) {
    std::cout << "%" << pair.first << " = " << "%" << pair.second.m_left << " "
              << expression_op_to_string(pair.second.m_operator) << " "
              << "%" << pair.second.m_right << std::endl;
  }

  std::cout << "Out index: " << std::to_string(data.out_index) << std::endl;
}
}  // namespace backend
