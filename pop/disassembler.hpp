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
