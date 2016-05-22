// astdump.cpp - This file is part of Pop
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
	auto mod = Pop::parse(std::cin, "<stdin>");
	Pop::DebugVisitor visitor(std::cout);
	mod->accept(visitor);
	return 0;
}
