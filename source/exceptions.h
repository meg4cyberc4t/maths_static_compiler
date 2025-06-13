#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <iostream>
#include <sstream>

// TODO(meg4cyberc4t): Make good exceptions

namespace _highlight_constants
{
inline static const std::string HIGHLIGHT_START = "\033[1;31m",
                                HIGHLIGHT_END = "\033[0m";
}

class unknown_literal_exception : public std::exception
{
public:
  unknown_literal_exception(const std::string& m_source,
                            std::size_t m_pos) noexcept
  {
    std::stringstream sstream;
    sstream << m_source.substr(0, m_pos)
            << _highlight_constants::HIGHLIGHT_START
            << m_source.substr(m_pos, 1) << _highlight_constants::HIGHLIGHT_END
            << m_source.substr(m_pos + 1, m_source.size() - m_pos + 1) << '\n'
            << _highlight_constants::HIGHLIGHT_START
            << std::string(m_pos, '-') + "^" << '\n'
            << "Unknown literal at position " << m_pos
            << _highlight_constants::HIGHLIGHT_END << '\n';
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
      : m_message(_highlight_constants::HIGHLIGHT_START + message
                  + _highlight_constants::HIGHLIGHT_END)
  {
  }

  auto what() const noexcept -> const char* override
  {
    return m_message.data();
  }

private:
  std::string m_message;
};

class control_flow_exception : public std::exception
{
public:
  explicit control_flow_exception(const std::string& message)
      : m_message(_highlight_constants::HIGHLIGHT_START + message
                  + _highlight_constants::HIGHLIGHT_END)
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
