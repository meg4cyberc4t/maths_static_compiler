#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>
#include <iostream>
#include <sstream>

class UnknownLiteralException : public std::exception
{
public:
  UnknownLiteralException(const std::string& source, std::size_t pos) noexcept
      : source(source)
  {
    this->pos = pos;
  }

  const char* what() const noexcept
  {
    const std::string &_highlight_start = "\033[1;31m",
                      &_highlight_end = "\033[0m";

    std::stringstream ss;
    ss << source.substr(0, pos) << _highlight_start << source.substr(pos, 1)
       << _highlight_end << source.substr(pos + 1, source.size() - pos + 1)
       << '\n'
       << _highlight_start << std::string(pos, '-') + "^" << '\n'
       << "Unknown literal at position " << pos << _highlight_end << '\n';
    std::cout << ss.str() << std::endl;
    return ss.str().data();
  }

protected:
  std::size_t pos;
  const std::string& source;
};

#endif