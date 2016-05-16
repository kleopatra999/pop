#ifndef POP_LOCATION_HPP
#define POP_LOCATION_HPP

#include <cstdint>

namespace Pop
{

struct SourcePosition
{
	std::uint32_t offset;
	std::uint32_t line;
	std::uint32_t column;
	SourcePosition(std::uint32_t offset = 0, std::uint32_t line = 0,
	               std::uint32_t column = 0)
	    : offset(offset), line(line), column(column)
	{
	}
};

struct SourceRange
{
	SourcePosition start;
	SourcePosition end;
	SourceRange() : start{0, 0, 0}, end{0, 0, 0}
	{
	}
	SourceRange(const SourcePosition &start, const SourcePosition &end)
	    : start(start), end(end)
	{
	}
};

// namespace Pop
}

#endif // POP_LOCATION_HPP
