// vm.cpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifdef HAVE_CONFIG_H
#include <pop/config.h>
#endif

#include <pop/pop.hpp>
#include <iostream>
#include <pop/pop.hpp>
#include <cstdio>
#include <string>

int main(int argc, char **argv)
{
	std::string code;
	while (true)
	{
		char ch = 0;
		std::cin.read(&ch, 1);
		if (std::cin.eof())
		{
			break;
		}
		else if (std::cin.bad())
		{
			std::cerr << "error: failed to read stdin" << std::endl;
			return 1;
		}
		else
		{
			code += ch;
		}
	}
	auto code_data = reinterpret_cast<const Pop::Uint8 *>(code.data());
	Pop::VM vm(code_data, code.size(), argc, argv);
	return vm.execute();
}
