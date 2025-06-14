#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <iostream>
#include <sstream>

class unknown_literal_error : public std::bad_exception
{
public:
  unknown_literal_error(const std::string& m_source, std::size_t m_pos) noexcept
  {
    std::stringstream sstream;
    sstream << m_source.substr(0, m_pos) << m_source.substr(m_pos, 1)
            << m_source.substr(m_pos + 1, m_source.size() - m_pos + 1) << '\n'
            << std::string(m_pos, '-') + "^" << '\n'
            << "Unknown literal at position " << m_pos << '\n';
    this->m_message = sstream.str();
  }

  auto what() const noexcept -> const char* override
  {
    return m_message.data();
  }

private:
  std::string m_message;
};

class parse_exception : public std::exception
{
public:
  explicit parse_exception(const std::string& message)
      : m_message(message)
  {
  }

  auto what() const noexcept -> const char* override
  {
    return m_message.data();
  }

private:
  std::string m_message;
};

class control_flow_error : public std::bad_exception
{
public:
  explicit control_flow_error(const std::string& message)
      : m_message(message)
  {
  }

  auto what() const noexcept -> const char* override
  {
    return m_message.data();
  }

private:
  std::string m_message;
};
#endif
