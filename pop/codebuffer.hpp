// codebuffer.hpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifndef POP_CODEBUFFER_HPP
#define POP_CODEBUFFER_HPP

#if !defined(POP_COMPILING) && !defined(POP_HPP_INCLUDED)
#error "Invalid individual include, include only the <pop/pop.hpp> header"
#endif

#include <pop/types.hpp>
#include <ostream>

namespace Pop
{

class CodeBuffer
{
public:
	std::ostream &out;

	CodeBuffer(std::ostream &out) : out(out)
	{
	}

	CodeBuffer &put_u8(Uint8 b)
	{
		out.write(reinterpret_cast<const char *>(&b), 1);
		return *this;
	}

	CodeBuffer &put_u16(Uint16 v)
	{
		put_u8((v >> 8) & 0xFF);
		put_u8(v & 0xFF);
		return *this;
	}

	CodeBuffer &put_u32(Uint32 v)
	{
		put_u8((v >> 24) & 0xFF);
		put_u8((v >> 16) & 0xFF);
		put_u8((v >> 8) & 0xFF);
		put_u8(v & 0xFF);
		return *this;
	}

	CodeBuffer &put_u64(Uint64 v)
	{
		put_u8((v >> 56ull) & 0xFFull);
		put_u8((v >> 48ull) & 0xFFull);
		put_u8((v >> 40ull) & 0xFFull);
		put_u8((v >> 32ull) & 0xFFull);
		put_u8((v >> 24ull) & 0xFFull);
		put_u8((v >> 16ull) & 0xFFull);
		put_u8((v >> 8ull) & 0xFFull);
		put_u8(v & 0xFFull);
		return *this;
	}

	CodeBuffer &put_f32(Float32 v)
	{
		union
		{
			Uint32 i;
			Float32 f;
		} x;
		x.f = v;
		return put_u32(x.i);
	}

	CodeBuffer &put_f64(Float64 v)
	{
		union
		{
			Uint64 i;
			Float64 f;
		} x;
		x.f = v;
		return put_u64(x.i);
	}

	CodeBuffer &put_addr(CodeAddr v)
	{
		if (sizeof(v) == 4)
			return put_u32(v);
		else
			return put_u64(v);
	}

	CodeBuffer &put_ident(const std::string &v)
	{
		put_u8(v.size());
		for (auto ch : v)
			put_u8(ch);
		return *this;
	}

	CodeBuffer &put_string(const std::string &v)
	{
		put_u32(v.size());
		for (auto ch : v)
			put_u8(ch);
		return *this;
	}
};

// namespace Pop
}

#endif // POP_CODEBUFFER_HPP
