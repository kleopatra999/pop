// vm.cpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifdef HAVE_CONFIG_H
#include <pop/config.h>
#endif

#include <pop/pop.hpp>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>

static void print_help(const char *argv0)
{
	std::string arg0(argv0);
	auto slash = arg0.rfind('/');
	if (slash != arg0.npos)
		arg0 = arg0.substr(slash + 1);
	std::cout
	    << "Usage: " << arg0
	    << " [options] [input files...]\n"
	       "\n"
	       "Options:\n"
	       "  -h, --help       show this message and exit\n"
	       "  input files...   input bytecode files (default empty for stdin)\n"
	       "\n"
	       "Written and maintained by Matthew Brush <mbrush@codebrainz.ca>\n";
}

static void read_file(std::istream &in, std::string &out)
{
	while (true)
	{
		char ch = 0;
		in.read(&ch, 1);
		if (in.eof())
		{
			break;
		}
		else if (in.bad())
		{
			std::cerr << "error: failed to read file" << std::endl;
			std::exit(EXIT_FAILURE);
		}
		else
		{
			out += ch;
		}
	}
}

int main(int argc, char **argv)
{
	std::vector<std::string> input_fns;

	for (int i = 1; i < argc; i++)
	{
		if (std::strcmp(argv[i], "-h") == 0 ||
		    std::strcmp(argv[i], "--help") == 0)
		{
			print_help(argv[0]);
			return 0;
		}
		else
		{
			input_fns.emplace_back(argv[i]);
		}
	}

	std::string code;
	if (input_fns.empty()) // read stdin
		read_file(std::cin, code);
	else // read input files
	{
		for (auto &fn : input_fns)
		{
			std::ifstream f(fn);
			read_file(f, code);
		}
	}

	// feed the code to a new VM instance and execute it
	auto code_data = reinterpret_cast<const Pop::Uint8 *>(code.data());
	Pop::VM vm(code_data, code.size());
	return vm.execute();
}
