#ifndef _PARSING_EXCEPTIONS_H
#define _PARSING_EXCEPTIONS_H

#include <exception>
#include <iostream>

class ParseException : public std::exception
{
public:
  ParseException(const std::string& message)
      : message(_highlight_start + message + _highlight_end)
  {
  }

  const char* what() const noexcept { return message.data(); }

private:
  const std::string _highlight_start = "\033[1;31m", _highlight_end = "\033[0m";
  std::string message;
};

#endif