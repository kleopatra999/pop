// vm.hpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifndef POP_VM_HPP
#define POP_VM_HPP

#include <pop/decoder.hpp>
#include <pop/opcodes.hpp>
#include <pop/types.hpp>
#include <pop/value.hpp>

namespace Pop
{

struct VM
{
	static constexpr int EXIT_PAUSED = -1;

	CodeAddr ip;
	Decoder dec;
	bool running;
	bool paused;
	int exit_code;
	int argc;
	char **argv;

	VM();
	VM(int argc, char **argv);
	VM(const Uint8 *code, CodeAddr len, int argc = 0, char **argv = nullptr);

	// Machine control
	int execute(const Uint8 *code, CodeAddr len);
	int execute();
	void pause();
	void resume();
	void exit(int exit_code = 0);
};

// namespace Pop
}

#endif // POP_VM_HPP
