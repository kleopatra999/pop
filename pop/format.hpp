#ifndef POP_FORMAT_HPP
#define POP_FORMAT_HPP

#include <cstdarg>
#include <string>

namespace Pop
{

std::string format(const char *fmt, ...);
std::string vformat(const char *fmt, va_list ap);

// namespace Pop
}

#endif // POP_FORMAT_HPP
