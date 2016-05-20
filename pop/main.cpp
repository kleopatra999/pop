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

	/*  // test lexer
	    Lexer lex(ss);
	    Token tok;
	    while (lex.next_token(tok) != TokenKind::END)
	    {
	        tok.debug(std::cout);
	    }
	*/

	Ast::ModulePtr mod;
	try
	{
		mod = parse(ss);
		// Ast::DebugVisitor dvisitor(std::cout);
		// mod->accept(dvisitor);
		CodeGenVisitor cvisitor;
		mod->accept(cvisitor);
		auto ops = cvisitor.finish();
		for (auto &op : ops)
			op->list(std::cout);
	}
	catch (SyntaxError &e)
	{
		std::cerr << "error: " << e.line() << ":" << e.column() << ": "
		          << e.what() << std::endl;
	}

	return 0;
}
