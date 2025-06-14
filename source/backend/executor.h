#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "control_flow_builder.h"

namespace backend
{
class executor
{
  std::map<ssa_position, double> memory;

  double input_variable(std::string& name)
  {
    std::cout << "Give a value to the variable \"" << name << "\" = ";
    std::string input_line;
    std::getline(std::cin, input_line);
    return std::stod(input_line);
  }

public:
  executor() {}

  double execute(control_flow_data data)
  {
    for (auto& [pos, define] : data.defines) {
      memory.insert(std::make_pair(pos, define));
      std::cout << "%" << pos << " = " << define << '\n';
    }
    for (auto& [pos, variable_name] : data.variables) {
      auto define = input_variable(variable_name);
      memory.insert(std::make_pair(pos, define));
      std::cout << "%" << pos << " = " << define << '\n';
    }
    while (true) {
      for (auto& [pos, expression] : data.expressions) {
        if (memory.contains(expression.get_left())
            && memory.contains(expression.get_right()))
        {
          double left = memory.at(expression.get_left());
          double right = memory.at(expression.get_right());
          double define;
          switch (expression.get_operator()) {
            case expression_op::add:
              define = left + right;
              break;
            case expression_op::subtract:
              define = left - right;
              break;
            case expression_op::multiply:
              define = left * right;
              break;
            case expression_op::divide:
              define = left / right;
              break;
          }
          memory.insert(std::make_pair(pos, define));
          std::cout << "%" << pos << " = " << "%" << expression.get_left()
                    << " " << expression_op_to_string(expression.get_operator())
                    << " " << "%" << expression.get_right() << " = " << define
                    << '\n';
          if (pos == data.out_index) {
            return define;
          }
        }
      }
    }
  }
};
}  // namespace backend

#endif
