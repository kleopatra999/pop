#include <pop/pop.hpp>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

static std::string format_bcode(const std::string bc)
{
	const size_t line_len = 60;
	size_t line_cnt = 0;
	std::string ccode("\t\t\"");
	for (auto ch : bc)
	{
		char chr_hex[9] = {0};
		size_t len = std::snprintf(chr_hex, 8, "\\x%02X", std::uint8_t(ch));
		ccode += chr_hex;
		line_cnt += len;
		if (line_cnt >= line_len)
		{
			ccode += "\"\n\t\t\"";
			line_cnt = 0;
		}
	}
	if (ccode.back() != '"')
		ccode += '"';
	return ccode;
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "error: not enough arguments, expecting filename(s)"
		          << std::endl;
		return 1;
	}

	std::stringstream bc;
	for (int i = 1; i < argc; i++)
	{
		std::ifstream f(argv[i]);
		Pop::compile(f, argv[i], bc);
	}

	std::string bcode(bc.str());
	std::string fbcode(format_bcode(bcode));

	std::printf(
	    "/* This file is auto-generated, do not edit. */\n"
	    "\n"
	    "#include <pop/pop.hpp>\n"
	    "\n"
	    "int main(int argc, char **argv)\n"
	    "{\n"
	    "\tstatic const unsigned char bc[] = (const unsigned char*)\n%s;\n"
	    "\tPop::VM vm(argc, argv);\n"
	    "\treturn vm.execute(bc, sizeof bc);\n"
	    "}\n",
	    fbcode.c_str());

	return 0;
}
