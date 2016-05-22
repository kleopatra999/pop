// tokdump.cpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifdef HAVE_CONFIG_H
#include <pop/config.h>
#endif

#include <pop/pop.hpp>
#include <iostream>

int main()
{
	Pop::Token tok;
	Pop::Lexer lex(std::cin, "<stdin>");
	while (lex.next_token(tok) != Pop::TokenKind::END)
		tok.debug(std::cout);
	return 0;
}
