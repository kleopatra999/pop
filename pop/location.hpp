// location.hpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifndef POP_LOCATION_HPP
#define POP_LOCATION_HPP

#include <pop/types.hpp>

namespace Pop
{

struct SourcePosition
{
	Uint32 offset;
	Uint32 line;
	Uint32 column;
	SourcePosition(Uint32 offset = 0, Uint32 line = 0, Uint32 column = 0)
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
