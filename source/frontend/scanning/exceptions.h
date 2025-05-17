#ifndef SCANNING_EXCEPTIONS_H_
#define SCANNING_EXCEPTIONS_H_

#include <exception>
#include <iostream>
#include <sstream>

class UnknownLiteralException : public std::exception
{
public:
  UnknownLiteralException(const std::string& source, std::size_t pos) noexcept
  {
    std::stringstream ss;
    ss << source.substr(0, pos) << _highlight_start << source.substr(pos, 1)
       << _highlight_end << source.substr(pos + 1, source.size() - pos + 1)
       << '\n'
       << _highlight_start << std::string(pos, '-') + "^" << '\n'
       << "Unknown literal at position " << pos << _highlight_end << '\n';
    this->message = ss.str();
  }

  const char* what() const noexcept { return message.data(); }

private:
  std::string message;

  const std::string _highlight_start = "\033[1;31m", _highlight_end = "\033[0m";
};

#endif