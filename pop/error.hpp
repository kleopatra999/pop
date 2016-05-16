#ifndef POP_ERROR_HPP
#define POP_ERROR_HPP

#include <cstdint>
#include <stdexcept>
#include <string>

namespace Pop
{

class Error : public std::runtime_error
{
protected:
	Error(const std::string &what) : std::runtime_error(what)
	{
	}
};

class RuntimeError : public Error
{
public:
	RuntimeError(const std::string &what) : Error(what)
	{
	}
};

class SyntaxError : public Error
{
public:
	SyntaxError(const std::string &what, std::uint32_t line = std::uint32_t(-1),
	            std::uint32_t column = std::uint32_t(-1))
	    : Error(what), line_(line), column_(column)
	{
	}

	std::uint32_t line() const
	{
		return line_;
	}
	std::uint32_t column() const
	{
		return column_;
	}

private:
	std::uint32_t line_;
	std::uint32_t column_;
};

// namespace Pop
}

#endif // POP_ERROR_HPP
