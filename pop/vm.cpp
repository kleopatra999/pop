// vm.cpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifdef HAVE_CONFIG_H
#include <pop/config.h>
#endif

#include <pop/vm.hpp>

namespace Pop
{

VM::VM() : VM(0, nullptr)
{
}

VM::VM(int argc, char **argv) : VM(nullptr, 0, 0, nullptr)
{
}

VM::VM(const Uint8 *code, CodeAddr len, int argc, char **argv)
    : ip(0), dec(&ip, code, len), running(false), paused(false), exit_code(0),
      argc(argc), argv(argv)
{
}

int VM::execute(const Uint8 *code, CodeAddr len)
{
	dec = Decoder(&ip, code, len);
	running = true;
	paused = false;
	exit_code = 0;

	while (running && !paused)
	{
		switch (auto op = dec.read_op())
		{
			case OpCode::OP_HALT:
				running = false;
				break;
			case OpCode::OP_NOP:
				break;
			default:
				break;
		}
	}

	return exit_code;
}

int VM::execute()
{
	if (dec.code)
		return execute(dec.code, dec.len);
	else
		return EXIT_FAILURE;
}

void VM::pause()
{
	if (running && !paused)
		paused = true;
	exit_code = EXIT_PAUSED;
}

void VM::resume()
{
	if (running && paused)
		paused = false;
}

void VM::exit(int exit_code_)
{
	if (running)
	{
		exit_code = exit_code_;
		running = false;
		paused = false;
	}
}

// namespace Pop
}
