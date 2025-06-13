#ifndef CONTROL_FLOW_BUILDER_H
#define CONTROL_FLOW_BUILDER_H

#include <iostream>
#include <map>
#include <set>

#include "frontend/parsing/expression.h"

#define MINUS_ONE_SSA_POSITION 0

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

class expression

{
  expression(ssa_position left, expression_op op, ssa_position right)
      : m_left(left)
      , m_operator(op)
      , m_right(right)
  {
  }

private:
  ssa_position m_left;
  expression_op m_operator;
  ssa_position m_right;

  friend class control_flow_builder;
};

class control_flow_builder
{
  struct control_flow_data
  {
    std::map<ssa_position, double> defines = {
        {MINUS_ONE_SSA_POSITION, -1},
    };  // %1 - число
    std::map<ssa_position, std::string> variables;  // %1 - название переменной
    std::map<ssa_position, backend::expression> expressions;  // %1 = %2 + %3
    std::map<ssa_position, std::set<ssa_position>>
        uses;  // %1 и где используется
    ssa_position control_flow_index = 1;
  };

  control_flow_data data;

public:
  explicit control_flow_builder() {}

  ssa_position add_expression(const frontend::expression& expr);
  ssa_position add_expression(const frontend::binary_expression& expr);
  ssa_position add_expression(const frontend::grouping_expression& expr);
  ssa_position add_expression(const frontend::number_expression& expr);
  ssa_position add_expression(const frontend::variable_expression& expr);
  ssa_position add_expression(const frontend::unary_expression& expr);

  void optimize();

  void execute();
};

}  // namespace backend

#endif
