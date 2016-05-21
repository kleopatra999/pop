#include <pop/pop.hpp>
#include <iostream>
#include <sstream>

using namespace Pop;

int main()
{
	std::stringstream ss;

	ss << "function fib(n) {\n"
	      "    if (n == 0)\n"
	      "        return 0;\n"
	      "    else if (n == 1)\n"
	      "        return 1;\n"
	      "    else\n"
	      "        return (fib(n-1) + fib(n-2));\n"
	      "}\n"
	      "fib(1);\n";

	/*
	ss << "let a=1;\n"
	      "a++;\n"
	      "print(a);\n";
	*/

	try
	{
		std::stringstream sout;
		compile(ss, "<string>", sout);
		InstructionList ops;
		disassemble(sout, ops);
		for (auto &op : ops)
			op->dis(std::cout);
	}
	catch (SyntaxError &e)
	{
		std::cerr << "error: " << e.line() << ":" << e.column() << ": "
		          << e.what() << std::endl;
	}

	return 0;
}
