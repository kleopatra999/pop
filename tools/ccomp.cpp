// ccomp.cpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifdef HAVE_CONFIG_H
#include <pop/config.h>
#endif

#include <pop/pop.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

static std::string format_bytecodes(std::istream &in)
{
	const size_t max_line_len = 60;
	size_t col = 0;
	int ch;
	std::string s("    \"");
	while ((ch = in.get()) != EOF)
	{
		char hex_buf[16] = {0};
		size_t len = std::snprintf(hex_buf, 15, "\\x%02X", ch);
		s += hex_buf;
		col += len;
		if (col >= max_line_len)
		{
			s += "\"\n    \"";
			col = 0;
		}
	}
	if (s.back() != '"')
		s += '"';
	return s;
}

int main()
{
	std::cout << "#include <pop/pop.hpp>\n"
	             "\n"
	             "int main(int argc, char **argv)\n"
	             "{\n"
	             "  static const unsigned char bc[] = (const unsigned char*)\n"
	          << format_bytecodes(std::cin)
	          << ";\n"
	             "  Pop::VM vm(argc, argv);\n"
	             "  return vm.execute(bc, sizeof bc);\n"
	             "}\n";

	return 0;
}
