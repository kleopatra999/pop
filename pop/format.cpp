// format.cpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifdef HAVE_CONFIG_H
#include <pop/config.h>
#endif

#include <pop/format.hpp>
#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <stdexcept>

namespace Pop
{

std::string format(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	auto s = vformat(fmt, ap);
	va_end(ap);
	return s;
}

std::string vformat(const char *fmt, va_list ap)
{
	int size = 100;
	std::string s(size, '\0');

	while (true)
	{
		va_list args;

		va_copy(args, ap);
		int n = std::vsnprintf(&s[0], size, fmt, args);
		va_end(args);

		if (n < 0)
		{
			std::stringstream ss;
			ss << "failed to format string: " << std::strerror(errno) << " ("
			   << errno << ")";
			throw std::runtime_error(ss.str());
		}

		if (n < size)
		{
			s.resize(n);
			return s;
		}

		size = n + 1;
		s.resize(size, '\0');
	}

	assert(false);
	return "";
}

// namespace Pop
}
