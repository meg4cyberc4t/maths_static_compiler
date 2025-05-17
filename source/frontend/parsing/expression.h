#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <iostream>
#include <memory>

#include <frontend/scanning/token.h>

namespace frontend
{
class expression
{
public:
  virtual ~expression() = default;
  virtual auto to_string(std::size_t indent) -> std::string = 0;

  friend bool operator==(const expression& a, const expression& b);
};

class binary_expression : public expression
{
public:
  binary_expression(std::unique_ptr<expression> left,
                    token token,
                    std::unique_ptr<expression> right)
      : m_left(std::move(left))
      , m_token(std::move(token))
      , m_right(std::move(right))
  {
  }

  auto to_string(std::size_t indent) -> std::string override;

private:
  const std::unique_ptr<expression> m_left;
  const token m_token;
  const std::unique_ptr<expression> m_right;
};

class grouping_expression : public expression
{
public:
  grouping_expression(std::unique_ptr<expression> expr)
      : m_expr(std::move(expr))
  {
  }

  auto to_string(std::size_t indent) -> std::string override;

private:
  const std::unique_ptr<expression> m_expr;
};

class number_expression : public expression
{
public:
  explicit number_expression(float value)
      : m_value(value)
  {
  }

  auto to_string(std::size_t indent) -> std::string override;

private:
  float m_value;
};

class variable_expression : public expression
{
public:
  explicit variable_expression(token token)
      : m_token(std::move(token))
  {
  }

  auto to_string(std::size_t indent) -> std::string override;

private:
  token m_token;
};

class unary_expression : public expression
{
public:
  unary_expression(std::unique_ptr<expression> expr, token token)
      : m_expr(std::move(expr))
      , m_token(std::move(token))
  {
  }

  auto to_string(std::size_t indent) -> std::string override;

private:
  const std::unique_ptr<expression> m_expr;
  const token m_token;
};

}  // namespace frontend
#endif
