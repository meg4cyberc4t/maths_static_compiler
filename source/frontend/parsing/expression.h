#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include <iostream>

#include <frontend/scanning/token.h>

namespace frontend
{

class Expression
{
public:
  virtual std::string to_string(int indent = 0) = 0;

  virtual ~Expression() = default;
};

class BinaryException : public Expression
{
public:
  BinaryException(std::unique_ptr<Expression> left,
                  Token token,
                  std::unique_ptr<Expression> right)
      : left(std::move(left))
      , token(token)
      , right(std::move(right))
  {
  }

  std::string to_string(int indent = 0) override
  {
    return "(BinaryException:\n" + std::string(indent, ' ') + "left – "
        + left->to_string(indent + 1) + ",\n" + std::string(indent, ' ')
        + "token – " + std::to_string(token.type) + ",\n"
        + std::string(indent, ' ') + "right – " + right->to_string(indent + 1)
        + ")";
  }

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

  std::string to_string(int indent = 0) override
  {
    return std::string(indent, ' ') + "(GroupingExpression: expr – "
        + expr->to_string(indent + 1) + ")";
  }

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

  std::string to_string(int indent = 0) override
  {
    return "(NumberExpression: value – " + std::to_string(value) + ")";
  }

private:
  float_t value;
};

class VariableExpression : public Expression
{
public:
  VariableExpression(Token token)
      : token(token)
  {
  }

  std::string to_string(int indent = 0) override
  {
    return "(VariableExpression: tokenLexeme – " + token.lexeme + ")";
  }

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

  std::string to_string(int indent = 0) override
  {
    return "(UnaryExpression: token – " + std::to_string(token.type)
        + ", expr – " + expr->to_string(indent + 1) + ")";
  }

private:
  std::unique_ptr<Expression> expr;
  Token token;
};

}  // namespace frontend
#endif
