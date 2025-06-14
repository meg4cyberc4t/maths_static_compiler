
#include "control_flow_builder.h"

#include "boost/json.hpp"
#include "exceptions.h"
#include "frontend/parsing/expression.h"

namespace backend
{

ssa_position control_flow_builder::add_expression(
    const frontend::expression& expr)
{
  ssa_position position;
  if (auto binary_expr_ptr =
          dynamic_cast<const frontend::binary_expression*>(&expr))
  {
    position = add_expression(*binary_expr_ptr);
  } else if (auto grouping_expr_ptr =
                 dynamic_cast<const frontend::grouping_expression*>(&expr))
  {
    position = add_expression(*grouping_expr_ptr);
  } else if (auto number_expr_ptr =
                 dynamic_cast<const frontend::number_expression*>(&expr))
  {
    position = add_expression(*number_expr_ptr);
  } else if (auto variable_expr_ptr =
                 dynamic_cast<const frontend::variable_expression*>(&expr))
  {
    position = add_expression(*variable_expr_ptr);
  } else if (auto unary_expr_ptr =
                 dynamic_cast<const frontend::unary_expression*>(&expr))
  {
    position = add_expression(*unary_expr_ptr);
  } else {
    throw control_flow_error(
        "No implemented for this expression");  // UNREACHABLE
  }
  m_data.out_index = m_data.control_flow_index - 1;
  return position;
};

ssa_position control_flow_builder::add_expression(
    const frontend::number_expression& expr)
{
  auto value = expr.get_value();
  m_data.uses[m_data.control_flow_index] = {};
  auto it =
      std::find_if(m_data.defines.begin(),
                   m_data.defines.end(),
                   [&](const auto& pair) { return pair.second == value; });
  if (it != m_data.defines.end()) {
    return it->first;
  } else {
    m_data.defines[m_data.control_flow_index] = value;
    return m_data.control_flow_index++;
  }
}

ssa_position control_flow_builder::add_expression(
    const frontend::variable_expression& expr)
{
  m_data.uses[m_data.control_flow_index] = {};
  m_data.variables[m_data.control_flow_index] = expr.get_token().get_lexeme();
  return m_data.control_flow_index++;
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
    throw control_flow_error(
        "Not implemented binary expression construction for this operator");
  }
  const auto b_expr =
      expression(left_position, expression_map.at(type), right_position);
  m_data.expressions.insert(std::make_pair(m_data.control_flow_index, b_expr));
  m_data.uses[left_position].insert(m_data.control_flow_index);
  m_data.uses[right_position].insert(m_data.control_flow_index);
  m_data.uses[m_data.control_flow_index] = {};
  return m_data.control_flow_index++;
}

ssa_position control_flow_builder::add_expression(
    const frontend::unary_expression& expr)
{
  ssa_position position = add_expression(*expr.get_expr());

  auto type = expr.get_token().get_type();
  if (type != frontend::token_type::subtract) {
    throw control_flow_error(
        "Not implemented unary expression construction for this operator");
  }
  const auto b_expr =
      expression(position, expression_op::multiply, MINUS_ONE_SSA_POSITION);
  m_data.expressions.insert(std::make_pair(m_data.control_flow_index, b_expr));
  m_data.uses[position].insert(m_data.control_flow_index);
  m_data.uses[MINUS_ONE_SSA_POSITION].insert(m_data.control_flow_index);
  m_data.uses[m_data.control_flow_index] = {};
  return m_data.control_flow_index++;
}

ssa_position control_flow_builder::add_expression(
    const frontend::grouping_expression& expr)
{
  return add_expression(*expr.get_expr());
}

void control_flow_builder::replace_position(ssa_position old_position,
                                            ssa_position new_position)
{
  for (auto dependency_pos : m_data.uses.at(old_position)) {
    m_data.expressions.at(dependency_pos)
        .replace_position(old_position, new_position);
  }
  m_data.uses.erase(old_position);
  if (m_data.variables.contains(old_position)) {
    m_data.variables.insert(
        std::make_pair(new_position, m_data.variables.at(old_position)));
    m_data.variables.erase(old_position);
  }
  if (m_data.defines.contains(old_position)) {
    m_data.defines.insert(
        std::make_pair(new_position, m_data.defines.at(old_position)));
    m_data.defines.erase(old_position);
  }
  if (m_data.out_index == old_position) {
    m_data.out_index = new_position;
  }
}

void control_flow_builder::copy_propagation()
{
  std::map<backend::expression, ssa_position> expressions_using = {};
  for (auto pair : m_data.expressions) {
    if (!expressions_using.contains(pair.second)) {
      expressions_using[pair.second] = pair.first;
    } else {
      replace_position(pair.first, expressions_using[pair.second]);
    }
  }
}

void control_flow_builder::algebraic_simplification()
{
  for (auto& [pos, expr] : m_data.expressions) {
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
      m_data.out_index,
  };
  while (!worked_positions.empty()) {
    auto pos = worked_positions.back();
    worked_positions.pop_back();
    marked_positions.insert(pos);
    if (m_data.expressions.contains(pos)) {
      auto expr = m_data.expressions.at(pos);
      worked_positions.push_back(expr.m_left);
      worked_positions.push_back(expr.m_right);
    }
  }
  for (ssa_position i = 0; i < m_data.control_flow_index; i++) {
    if (marked_positions.contains(i)) {
      continue;
    }
    if (m_data.expressions.contains(i)) {
      auto expr = m_data.expressions.at(i);
      m_data.uses[expr.m_left].erase(i);
      m_data.uses[expr.m_right].erase(i);
      m_data.expressions.erase(i);
    }
    m_data.variables.erase(i);
    m_data.defines.erase(i);
    m_data.uses.erase(i);
  }
}

boost::json::object control_flow_data::to_json() const
{
  boost::json::object obj;
  for (auto& [position, define] : defines) {
    obj["%" + std::to_string(position)] = std::to_string(define);
  }
  for (auto& [position, variable] : variables) {
    obj["%" + std::to_string(position)] = variable;
  }
  for (auto& [position, expr] : expressions) {
    obj["%" + std::to_string(position)] = "%" + std::to_string(expr.get_left())
        + " " + expression_op_to_string(expr.get_operator()) + " %"
        + std::to_string(expr.get_right());
  }
  return obj;
};

}  // namespace backend
