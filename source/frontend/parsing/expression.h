#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include <iostream>

#include <frontend/scanning/token.h>

namespace frontend
{

class Expression
{
public:
  virtual ~Expression() = default;
  virtual std::string to_string(int indent = 0) = 0;
};

class BinaryExpression : public Expression
{
public:
  BinaryExpression(std::unique_ptr<Expression> left,
                   Token token,
                   std::unique_ptr<Expression> right)
      : left(std::move(left))
      , token(token)
      , right(std::move(right))
  {
  }

  std::string to_string(int indent = 0) override;

private:
  std::unique_ptr<Expression> left;
  Token token;
  std::unique_ptr<Expression> right;
};

class GroupingExpression : public Expression
{
public:
  GroupingExpression(std::unique_ptr<Expression> expr)
      : expr(std::move(expr))
  {
  }

  std::string to_string(int indent = 0) override;

private:
  std::unique_ptr<Expression> expr;
};

class NumberExpression : public Expression
{
public:
  NumberExpression(float_t value)
      : value(value)
  {
  }

  std::string to_string(int indent = 0) override;

private:
  float value;
};

class VariableExpression : public Expression
{
public:
  VariableExpression(Token token)
      : token(token)
  {
  }

  std::string to_string(int indent = 0) override;

private:
  Token token;
};

class UnaryExpression : public Expression
{
public:
  UnaryExpression(std::unique_ptr<Expression> expr, Token token)
      : expr(std::move(expr))
      , token(token)
  {
  }

  std::string to_string(int indent = 0) override;

private:
  std::unique_ptr<Expression> expr;
  Token token;
};

}  // namespace frontend
#endif
