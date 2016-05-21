#include <pop/pop.hpp>
#include <iostream>

int main()
{
	Pop::InstructionList instructions;
	Pop::disassemble(std::cin, instructions);
	for (auto &instruction : instructions)
		instruction->dis(std::cout);
	return 0;
}
