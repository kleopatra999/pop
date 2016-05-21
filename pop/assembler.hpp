#ifndef POP_ASSEMBLER_HPP
#define POP_ASSEMBLER_HPP

#include <pop/ast.hpp>
#include <pop/transformer.hpp>
#include <ostream>

namespace Pop
{

void assemble(ModulePtr &mod, std::ostream &out);

// namespace Pop
}

#endif // POP_ASSEMBLER_HPP
