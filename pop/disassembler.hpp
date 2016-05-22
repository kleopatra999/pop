// disassembler.hpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifndef POP_DISASSEMBLER_HPP
#define POP_DISASSEMBLER_HPP

#include <pop/instructions.hpp>
#include <istream>

namespace Pop
{

void disassemble(std::istream &inp, InstructionList &out);

// namespace Pop
}

#endif // POP_DISASSEMBLER_HPP
