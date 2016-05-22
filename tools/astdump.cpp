#ifdef HAVE_CONFIG_H
#include <pop/config.h>
#endif

#include <pop/pop.hpp>
#include <iostream>

int main()
{
	auto mod = Pop::parse(std::cin, "<stdin>");
	Pop::DebugVisitor visitor(std::cout);
	mod->accept(visitor);
	return 0;
}
