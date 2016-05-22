#ifdef HAVE_CONFIG_H
#include <pop/config.h>
#endif

#include <pop/pop.hpp>
#include <iostream>

int main()
{
	Pop::compile(std::cin, "<stdin>", std::cout);
	return 0;
}
