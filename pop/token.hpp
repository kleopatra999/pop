// token.hpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifndef POP_TOKEN_HPP
#define POP_TOKEN_HPP

#include <pop/location.hpp>
#include <pop/types.hpp>
#include <ostream>
#include <string>
#include <vector>

namespace Pop
{

enum class TokenKind : Uint8
{
	END = 0,

	COLON = ':',
	SEMICOLON = ';',
	COMMA = ',',
	MEMBER = '.',
	LBRACE = '{',
	RBRACE = '}',
	LPAREN = '(',
	RPAREN = ')',
	LBRACKET = '[',
	RBRACKET = ']',

	SINGLE_LINE_COMMENT = 128,
	MULTI_LINE_COMMENT,

	// literals
	NULL_LITERAL,
	TRUE_LITERAL,
	FALSE_LITERAL,
	INT_LITERAL,
	FLOAT_LITERAL,
	STRING_LITERAL,
	IDENTIFIER,

	// keywords
	BREAK,
	CATCH,
	CONTINUE,
	DO,
	ELSE,
	FALSE,
	FINALLY,
	FOR,
	FUNCTION,
	GOTO,
	IF,
	IN,
	LET,
	NUL,
	RETURN,
	THROW,
	TRUE,
	TRY,
	UNLESS,
	UNTIL,
	WHILE,

	// arithmetic ops
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,
	POW,
	UPLUS,
	UMINUS,

	// logical ops
	L_AND,
	L_OR,
	L_NOT,

	// bitwise ops
	B_AND,
	B_OR,
	B_XOR,
	B_NOT,
	LSHIFT,
	RSHIFT,

	// in-places ops
	ADD_ASSIGN,
	SUB_ASSIGN,
	MUL_ASSIGN,
	DIV_ASSIGN,
	MOD_ASSIGN,
	POW_ASSIGN,
	AND_ASSIGN,
	OR_ASSIGN,
	XOR_ASSIGN,
	NOT_ASSIGN,
	LEFT_ASSIGN,
	RIGHT_ASSIGN,
	ASSIGN,
	INCREMENT,
	DECREMENT,
	PREINC,
	PREDEC,
	POSTINC,
	POSTDEC,

	// comparisons
	EQ,
	NE,
	GT,
	GE,
	LT,
	LE,

	ERROR = 255,
};

const char *token_kind_name(TokenKind kind);

struct Token
{
	TokenKind kind;
	SourceRange range;
	std::string text;

	Token() : kind(TokenKind::ERROR)
	{
	}

	const char *kind_name()
	{
		return token_kind_name(kind);
	}

	void reset()
	{
		kind = TokenKind::ERROR;
		range.start = SourcePosition{0, 0, 0};
		range.end = SourcePosition{0, 0, 0};
		text.clear();
	}

	void debug(std::ostream &out)
	{
		out << "<token id=\"" << int(kind) << "\" name=\"" << kind_name()
		    << "\" text =\"" << text << "\" line=\"" << range.start.line
		    << "\" column=\"" << range.start.column << "\">\n";
	}
};

typedef std::vector<Token> TokenList;

// namespace Pop
}

#endif // POP_TOKEN_HPP
