// format.hpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifndef POP_FORMAT_HPP
#define POP_FORMAT_HPP

#if !defined(POP_COMPILING) && !defined(POP_HPP_INCLUDED)
#error "Invalid individual include, include only the <pop/pop.hpp> header"
#endif

#include <cstdarg>
#include <string>

namespace Pop
{

std::string format(const char *fmt, ...);
std::string vformat(const char *fmt, va_list ap);

// namespace Pop
}

#endif // POP_FORMAT_HPP
