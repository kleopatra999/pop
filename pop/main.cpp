#include <pop/pop.hpp>
#include <iostream>
#include <sstream>

using namespace Pop;

int main()
{
	std::stringstream ss;
	ss << "let foo = 1+2;\n";

	Ast::ModulePtr mod;
	try
	{
		mod = parse(ss);
		Ast::DebugVisitor visitor(std::cout);
		mod->accept(visitor);
	}
	catch (SyntaxError &e)
	{
		std::cerr << "error: " << e.line() << ":" << e.column() << ": "
		          << e.what() << std::endl;
	}

	return 0;
}
