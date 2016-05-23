// lexer.hpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifndef POP_LEXER_HPP
#define POP_LEXER_HPP

#if !defined(POP_COMPILING) && !defined(POP_HPP_INCLUDED)
#error "Invalid individual include, include only the <pop/pop.hpp> header"
#endif

#include <pop/token.hpp>
#include <cstdint>
#include <istream>
#include <vector>

namespace Pop
{

class Lexer
{
public:
	const char *filename;
	int chr;
	TokenKind kind;
	std::uint32_t offset;
	std::uint32_t line;
	std::uint32_t column;
	std::string text;
	std::istream &inp;

	Lexer(std::istream &inp, const char *fn = "<stream>")
	    : filename(fn), chr(' '), offset(std::uint32_t(-1)), line(1),
	      column(std::uint32_t(-1)), inp(inp)
	{
	}

	TokenKind next_token(Token &t);

private:
	int getch();
	void reset();
	void start_token(Token &out_token);
	TokenKind end_token(TokenKind k, Token &out_token);
};

static inline TokenList tokenize(std::istream &in, const char *fn = "<stream>")
{
	Lexer lex(in, fn);
	TokenList tokens;
	TokenKind kind;
	do
	{
		Token tok;
		kind = lex.next_token(tok);
		tokens.push_back(std::move(tok));
	} while (kind != TokenKind::END && kind != TokenKind::ERROR);
	return tokens;
}

// namespace Pop
}

#endif // POP_LEXER_HPP
