// test_lexer.cpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifdef HAVE_CONFIG_H
#include <pop/config.h>
#endif

#include <pop/pop.hpp>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace Pop;
using TK = TokenKind;

struct TestCodes
{
	std::string code;
	std::vector<TokenKind> kinds;
	std::vector<std::string> texts;
};

// clang-format off

static const TestCodes test_codes[] =
{
	{ ":", { TK::COLON }, { "" } },
	{ ";", { TK::SEMICOLON }, { "" } },
	{ ",", { TK::COMMA }, { "" } },
	{ ".", { TK::MEMBER }, { "" } },
	{ "{", { TK::LBRACE }, { "" } },
	{ "}", { TK::RBRACE }, { "" } },
	{ "(", { TK::LPAREN }, { "" } },
	{ ")", { TK::RPAREN }, { "" } },
	{ "[", { TK::LBRACKET }, { "" } },
	{ "]", { TK::RBRACKET }, { "" } },
	{ "// abc", { TK::SINGLE_LINE_COMMENT }, { "// abc" } },
	{ "/* abc */", { TK::MULTI_LINE_COMMENT }, { "/* abc */" } },
	{ "123", { TK::INT_LITERAL }, { "123" } },
	{ "0123", { TK::INT_LITERAL }, { "0123" } }, // octal
	{ "0x123", { TK::INT_LITERAL }, { "0x123" } },
	{ "0b101", { TK::INT_LITERAL }, { "0b101" } },
	{ "0o123", { TK::INT_LITERAL }, { "0o123" } },
	{ "0d123", { TK::INT_LITERAL }, { "0d123" } },
	{ "123.456", { TK::FLOAT_LITERAL }, { "123.456" } },
	{ "0123.456", { TK::FLOAT_LITERAL }, { "0123.456" } }, // octal
	{ "0x123.456", { TK::FLOAT_LITERAL }, { "0x123.456" } },
	{ "0b101.111", { TK::FLOAT_LITERAL }, { "0b101.111" } },
	{ "0o123.456", { TK::FLOAT_LITERAL }, { "0o123.456" } },
	{ "0d123.456", { TK::FLOAT_LITERAL }, { "0d123.456" } },
	{ "\"abc\"", { TK::STRING_LITERAL }, { "abc" } },
	{ "\"ab\\\"c\"", { TK::STRING_LITERAL }, { "ab\"c" } },
	{ "'abc'", { TK::STRING_LITERAL }, { "abc" } },
	{ "'ab\\'c'", { TK::STRING_LITERAL }, { "ab'c" } },
	{ "abc", { TK::IDENTIFIER }, { "abc" } },
	{ "_abc123", { TK::IDENTIFIER }, { "_abc123" } },
	{ "123abc", { TK::INT_LITERAL, TK::IDENTIFIER }, { "123", "abc" } },
	{ "break", { TK::BREAK }, { "" } },
	{ "catch", { TK::CATCH }, { "" } },
	{ "continue", { TK::CONTINUE }, { "" } },
	{ "do", { TK::DO }, { "" } },
	{ "else", { TK::ELSE }, { "" } },
	{ "false", { TK::FALSE }, { "" } },
	{ "finally", { TK::FINALLY }, { "" } },
	{ "for", { TK::FOR }, { "" } },
	{ "function", { TK::FUNCTION }, { "" } },
	{ "goto", { TK::GOTO }, { "" } },
	{ "if", { TK::IF }, { "" } },
	{ "in", { TK::IN }, { "" } },
	{ "let", { TK::LET }, { "" } },
	{ "null", { TK::NUL }, { "" } },
	{ "return", { TK::RETURN }, { "" } },
	{ "throw", { TK::THROW }, { "" } },
	{ "true", { TK::TRUE }, { "" } },
	{ "try", { TK::TRY }, { "" } },
	{ "unless", { TK::UNLESS }, { "" } },
	{ "until", { TK::UNTIL }, { "" } },
	{ "while", { TK::WHILE }, { "" } },
	{ "+", { TK::ADD }, { "" } },
	{ "-", { TK::SUB }, { "" } },
	{ "*", { TK::MUL }, { "" } },
	{ "/", { TK::DIV }, { "" } },
	{ "%", { TK::MOD }, { "" } },
	{ "**", { TK::POW }, { "" } },
	{ "&&", { TK::L_AND }, { "" } },
	{ "||", { TK::L_OR }, { "" } },
	{ "!", { TK::L_NOT }, { "" } },
	{ "&", { TK::B_AND }, { "" } },
	{ "|", { TK::B_OR }, { "" } },
	{ "^", { TK::B_XOR }, { "" } },
	{ "~", { TK::B_NOT }, { "" } },
	{ "<<", { TK::LSHIFT }, { "" } },
	{ ">>", { TK::RSHIFT }, { "" } },
	{ "+=", { TK::ADD_ASSIGN }, { "" } },
	{ "-=", { TK::SUB_ASSIGN }, { "" } },
	{ "*=", { TK::MUL_ASSIGN }, { "" } },
	{ "/=", { TK::DIV_ASSIGN }, { "" } },
	{ "%=", { TK::MOD_ASSIGN }, { "" } },
	{ "**=", { TK::POW_ASSIGN }, { "" } },
	{ "&=", { TK::AND_ASSIGN }, { "" } },
	{ "|=", { TK::OR_ASSIGN }, { "" } },
	{ "^=", { TK::XOR_ASSIGN }, { "" } },
	{ "~=", { TK::NOT_ASSIGN }, { "" } },
	{ "<<=", { TK::LEFT_ASSIGN }, { "" } },
	{ ">>=", { TK::RIGHT_ASSIGN }, { "" } },
	{ "=", { TK::ASSIGN }, { "" } },
	{ "++", { TK::INCREMENT }, { "" } },
	{ "--", { TK::DECREMENT }, { "" } },
	{ "==", { TK::EQ }, { "" } },
	{ "!=", { TK::NE }, { "" } },
	{ ">", { TK::GT }, { "" } },
	{ ">=", { TK::GE }, { "" } },
	{ "<", { TK::LT }, { "" } },
	{ "<=", { TK::LE }, { "" } },
	// multi-operators
	{ "====", { TK::EQ, TK::EQ }, { "", "" } },
	{ "===", { TK::EQ, TK::ASSIGN }, { "", "" } },
	{ "***==", { TK::POW, TK::MUL_ASSIGN, TK::ASSIGN }, { "", "", "" } },
	// multi-token test
	{ "if (a < 1) { a++; }", {
		TK::IF,
		TK::LPAREN,
		TK::IDENTIFIER,
		TK::LT,
		TK::INT_LITERAL,
		TK::RPAREN,
		TK::LBRACE,
		TK::IDENTIFIER,
		TK::INCREMENT,
		TK::SEMICOLON,
		TK::RBRACE
	}, { "", "", "a", "", "1", "", "", "a", "", "", "" } },
};

// clang-format on

int main()
{
	// join the test codes into a single newline separated string
	std::stringstream ss;
	for (auto &tst : test_codes)
		ss << tst.code << "\n";

	try
	{
		auto tokens = tokenize(ss, "<test>");
		size_t i = 0, index = 0;
		const size_t n = sizeof(test_codes) / sizeof(TestCodes);
		for (i = 0; i < n; i++)
		{
			auto &test = test_codes[i];
			assert(test.kinds.size() == test.texts.size());
			for (size_t j = 0; j < test.kinds.size(); j++)
			{
				size_t new_index = index + 1;
				assert(new_index < tokens.size());
				index = new_index;
				auto &tok = tokens[index];
				if (tok.kind == TK::END)
					break;
				assert(tok.kind != TK::ERROR);
				if (tok.kind != test.kinds[j])
				{
					std::cerr << "wrong kind in '" << test.code
					          << "' test index '" << i << "'. Expected '"
					          << token_kind_name(test.kinds[j]) << "' got '"
					          << token_kind_name(tok.kind) << "'" << std::endl;
					break;
				}
				else if (tok.text != test.texts[j])
				{
					std::cerr << "wrong text in '" << test.code
					          << "' test index '" << i << "'. Expected '"
					          << test.texts[j] << "' got '" << tok.text << "'"
					          << std::endl;
					break;
				}
			}
		}
		assert(i == n);
	}
	catch (SyntaxError &e)
	{
		std::cerr << "error: " << e.line() << ":" << e.column() << ": "
		          << e.what() << std::endl;
		return 1;
	}
	return 0;
}
