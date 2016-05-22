// assembler.hpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

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
