#ifndef CONTROL_FLOW_BUILDER_H
#define CONTROL_FLOW_BUILDER_H

#include <iostream>
#include <map>
#include <set>

#include "frontend/parsing/expression.h"

typedef unsigned long ssa_position;

namespace backend
{

enum expression_op
{
  add,
  subtract,
  divide,
  multiply,
};

static char expression_op_to_string(expression_op op)
{
  static const std::array<const char, 4> mapper = {'+', '-', '/', '*'};
  return mapper[static_cast<int>(op)];
}

class expression

{
public:
  expression(ssa_position left, expression_op op, ssa_position right)
      : m_left(left)
      , m_operator(op)
      , m_right(right)
  {
  }

  constexpr auto operator<=>(const expression& other) const
  {
    return std::tie(m_left, m_operator, m_right)
        <=> std::tie(other.m_left, other.m_operator, other.m_right);
  }

  constexpr ssa_position get_left() const { return m_left; }

  constexpr expression_op get_operator() const { return m_operator; }

  constexpr ssa_position get_right() const { return m_right; }

private:
  void replace_position(ssa_position old_pos, ssa_position new_pos)
  {
    if (m_left == old_pos) {
      m_left = new_pos;
    }
    if (m_right == old_pos) {
      m_right = new_pos;
    }
  }

  bool is_one_of_positions_equal(ssa_position pos)
  {
    return (m_left == pos) || (m_right == pos);
  }

  ssa_position m_left;
  expression_op m_operator;
  ssa_position m_right;

  friend class control_flow_builder;
};

struct control_flow_data
{
  std::map<ssa_position, double> defines = {
#define MINUS_ONE_SSA_POSITION 0
      {MINUS_ONE_SSA_POSITION, -1},
#define ZERO_SSA_POSITION 1
      {ZERO_SSA_POSITION, 0},
#define ONE_SSA_POSITION 2
      {ONE_SSA_POSITION, 1},
  };
  std::map<ssa_position, std::string> variables;
  std::map<ssa_position, backend::expression> expressions;
  std::map<ssa_position, std::set<ssa_position>> uses;
  // decision tree;
  ssa_position control_flow_index = 3;
  ssa_position out_index;
};

class control_flow_builder
{
  control_flow_data m_data;

  ssa_position add_expression(const frontend::expression& expr);
  ssa_position add_expression(const frontend::binary_expression& expr);
  ssa_position add_expression(const frontend::grouping_expression& expr);
  ssa_position add_expression(const frontend::number_expression& expr);
  ssa_position add_expression(const frontend::variable_expression& expr);
  ssa_position add_expression(const frontend::unary_expression& expr);

  void replace_position(ssa_position old_position, ssa_position new_position);

  void copy_propagation();
  void algebraic_simplification();
  void dead_code_elimination();

public:
  explicit control_flow_builder(const frontend::expression& expr)
  {
    add_expression(expr);
    copy_propagation();
    algebraic_simplification();
    dead_code_elimination();
  }

  control_flow_data get_data() const { return m_data; }
};

}  // namespace backend

#endif
