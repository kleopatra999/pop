// error.hpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifndef POP_ERROR_HPP
#define POP_ERROR_HPP

#include <pop/types.hpp>
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
	SyntaxError(const std::string &what, Uint32 line = Uint32(-1),
	            Uint32 column = Uint32(-1))
	    : Error(what), line_(line), column_(column)
	{
	}

	Uint32 line() const
	{
		return line_;
	}
	Uint32 column() const
	{
		return column_;
	}

private:
	Uint32 line_;
	Uint32 column_;
};

// namespace Pop
}

#endif // POP_ERROR_HPP
