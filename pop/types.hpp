// types.hpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifndef POP_TYPES_HPP
#define POP_TYPES_HPP

#if !defined(POP_COMPILING) && !defined(POP_HPP_INCLUDED)
#error "Invalid individual include, include only the <pop/pop.hpp> header"
#endif

#include <cstdint>

namespace Pop
{

typedef std::uint8_t Uint8;
typedef std::uint16_t Uint16;
typedef std::uint32_t Uint32;
typedef std::uint64_t Uint64;
typedef std::int64_t Int64;
typedef float Float32;
typedef double Float64;
typedef std::uint32_t CodeAddr;

// namespace Pop
}

#endif // POP_TYPES_HPP
