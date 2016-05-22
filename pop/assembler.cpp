// assembler.cpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifdef HAVE_CONFIG_H
#include <pop/config.h>
#endif

#include <pop/assembler.hpp>
#include <pop/error.hpp>
#include <iostream>
#include <sstream>

namespace Pop
{

void assemble(ModulePtr &mod, std::ostream &out)
{
	LabelMap labels;
	InstructionList ops;
	CodeAddr offset = 0;

	// first pass to resolve then drop labels
	for (auto &op : transform(mod))
	{
		if (op->code == OpCode::OP_LABEL)
		{
			auto &name = static_cast<Label *>(op.get())->name;
			auto res = labels.emplace(name, offset);
			if (!res.second)
			{
				std::stringstream ss;
				ss << "multiple labels named '" << name << "'";
				throw RuntimeError(ss.str());
			}
		}
		else
		{
			offset += op->size();
			ops.push_back(std::move(op));
		}
	}

	// second pass to generate byte code
	std::stringstream bcout;
	CodeBuffer bcbuf(bcout);
	for (auto &op : ops)
		op->codegen(bcbuf, labels);
	out << bcout.str();
}

// namespace Pop
}
