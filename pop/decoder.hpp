// decoder.hpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifndef POP_DECODER_HPP
#define POP_DECODER_HPP

#if !defined(POP_COMPILING) && !defined(POP_HPP_INCLUDED)
#error "Invalid individual include, include only the <pop/pop.hpp> header"
#endif

#include <pop/opcodes.hpp>
#include <pop/types.hpp>
#include <cassert>
#include <string>

namespace Pop
{

struct Decoder
{
	CodeAddr *ip;
	const Uint8 *code;
	CodeAddr len;

	Decoder(CodeAddr *ip, const Uint8 *code, CodeAddr len)
	    : ip(ip), code(code), len(len)
	{
		assert(code);
		assert(len > 0);
	}

	template <class T>
	inline T read_byte_as()
	{
		assert(*ip < len);
		return static_cast<T>(code[(*ip)++]);
	}

	OpCode read_op()
	{
		return read_byte_as<OpCode>();
	}

	CodeAddr read_addr()
	{
		// FIXME: make this a compile-time check
		if (sizeof(CodeAddr) == 4)
			return read_u32();
		else
			return read_u64();
	}

	Uint8 read_u8()
	{
		return read_byte_as<Uint8>();
	}

	Uint16 read_u16()
	{
		return (read_byte_as<Uint16>() << 8) | read_byte_as<Uint16>();
	}

	Uint32 read_u32()
	{
		return (read_byte_as<Uint32>() << 24) | (read_byte_as<Uint32>() << 16) |
		       (read_byte_as<Uint32>() << 8) | read_byte_as<Uint32>();
	}

	Uint64 read_u64()
	{
		return (read_byte_as<Uint64>() << 56ULL) |
		       (read_byte_as<Uint64>() << 48ULL) |
		       (read_byte_as<Uint64>() << 40ULL) |
		       (read_byte_as<Uint64>() << 32ULL) |
		       (read_byte_as<Uint64>() << 24ULL) |
		       (read_byte_as<Uint64>() << 16ULL) |
		       (read_byte_as<Uint64>() << 8ULL) | read_byte_as<Uint64>();
	}

	Float32 read_f32()
	{
		union
		{
			Uint32 i;
			Float32 f;
		} v;
		v.i = read_u32();
		return v.f;
	}

	Float64 read_f64()
	{
		union
		{
			Uint64 i;
			Float64 f;
		} v;
		v.i = read_u64();
		return v.f;
	}

	std::string read_string()
	{
		std::string s;
		auto len = read_u32();
		s.reserve(len);
		for (auto i = 0u; i < len; i++)
			s += read_byte_as<char>();
		return s;
	}

	std::string read_name()
	{
		std::string s;
		auto len = read_u8();
		s.reserve(len);
		for (auto i = 0u; i < len; i++)
			s += read_byte_as<char>();
		return s;
	}
};

// namespace Pop
}

#endif // POP_DECODER_HPP
