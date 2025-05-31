#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

#include <frontend/scanning/token.h>

namespace frontend
{
class expression
{
public:
  virtual ~expression() = default;

  virtual auto to_string(std::size_t indent) -> std::string = 0;

  virtual bool operator==(const expression& other) const { return true; }
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

  bool operator==(const expression& other) const override
  {
    const binary_expression* other_casted =
        static_cast<const binary_expression*>(&other);
    if (!other_casted) {
      return false;  // Not the equal type
    }
    return expression::operator==(other)
        && (*m_left == *other_casted->m_left.get()
            && *m_right == *other_casted->m_right.get()
            && m_token == other_casted->m_token);
  }

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

  bool operator==(const expression& other) const override
  {
    const grouping_expression* other_casted =
        static_cast<const grouping_expression*>(&other);
    if (!other_casted) {
      return false;  // Not the equal type
    }
    return expression::operator==(other)
        && *m_expr == *other_casted->m_expr.get();
  }

private:
  const std::unique_ptr<expression> m_expr;
};

class number_expression : public expression
{
public:
  explicit number_expression(long double value)
      : m_value(value)
  {
  }

  auto to_string(std::size_t indent) -> std::string override;

  bool operator==(const expression& other) const override
  {
    const number_expression* other_casted =
        static_cast<const number_expression*>(&other);
    if (!other_casted) {
      return false;  // Not the equal type
    }
    return expression::operator==(other)
        && (std::fabsl(m_value - other_casted->m_value)
            < std::numeric_limits<long double>::epsilon());
  }

private:
  long double m_value;
};

class variable_expression : public expression
{
public:
  explicit variable_expression(token token)
      : m_token(std::move(token))
  {
  }

  auto to_string(std::size_t indent) -> std::string override;

  bool operator==(const expression* other) const
  {
    if (auto other_ptr = static_cast<const variable_expression*>(other)) {
      return m_token == other_ptr->m_token;
    }
    return false;
  }

  bool operator==(const expression& other) const override
  {
    const variable_expression* other_casted =
        static_cast<const variable_expression*>(&other);
    if (!other_casted) {
      return false;  // Not the equal type
    }
    return expression::operator==(other) && (m_token == other_casted->m_token);
  }

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

  bool operator==(const expression& other) const override
  {
    const unary_expression* other_casted =
        static_cast<const unary_expression*>(&other);
    if (!other_casted) {
      return false;  // Not the equal type
    }
    return expression::operator==(other)
        && (*m_expr == *other_casted->m_expr.get()
            && m_token == other_casted->m_token);
  }

private:
  const std::unique_ptr<expression> m_expr;
  const token m_token;
};

}  // namespace frontend
#endif
