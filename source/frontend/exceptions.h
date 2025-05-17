#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <iostream>
#include <sstream>

namespace
{
static constexpr const char *highlight_start = "\033[1;31m",
                            *highlight_end = "\033[0m";
}

class unknown_literal_exception : public std::exception
{
public:
  unknown_literal_exception(const std::string& m_source,
                            std::size_t m_pos) noexcept
  {
    std::stringstream sstream;
    sstream << m_source.substr(0, m_pos) << highlight_start
            << m_source.substr(m_pos, 1) << highlight_end
            << m_source.substr(m_pos + 1, m_source.size() - m_pos + 1) << '\n'
            << highlight_start << std::string(m_pos, '-') + "^" << '\n'
            << "Unknown literal at position " << m_pos << highlight_end << '\n';
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
      : m_message(highlight_start + message + highlight_end)
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