#ifndef POP_COMPILER_HPP
#define POP_COMPILER_HPP

#include <pop/assembler.hpp>
#include <pop/parser.hpp>
#include <pop/transformer.hpp>
#include <iostream>
#include <string>

namespace Pop
{

inline void compile(std::istream &inp, const std::string &inp_name,
                    std::ostream &out)
{
	auto mod = parse(inp, inp_name.c_str());
	assemble(mod, out);
}

inline void compile(std::istream &inp, std::ostream &out)
{
	compile(inp, "<stream>", out);
}

inline void ccompile(std::istream &inp, const std::string &inp_name,
                     std::ostream &out)
{
	out << "#include <pop/pop.hpp>\n"
	       "\n"
	       "int main()\n"
	       "{\n"
	       "\tINIT_VM();\n";
	auto mod = parse(inp, inp_name.c_str());
	auto ops = transform(mod);
	for (auto &op : ops)
		op->ccodegen(out);
	out << "\tEXIT_VM();\n"
	       "}\n";
}

inline void ccompile(std::istream &inp, std::ostream &out)
{
	ccompile(inp, "<stream>", out);
}

// namespace Pop
}

#endif // POP_COMPILER_HPP
