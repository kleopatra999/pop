#include <pop/pop.hpp>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace Pop;

typedef std::string String;
typedef std::vector<String> StringList;

struct Options
{
	String program_name;
	String output_file;
	String listing_file;
	StringList input_files;
};

static void parse_args(int argc, char **argv, Options &opts);

int main(int argc, char **argv)
{
	Options opts;
	parse_args(argc, argv, opts);

	try
	{
		std::stringstream out;
		if (opts.input_files.empty())
			compile(std::cin, "<string>", out);
		else
		{
			for (auto &fn : opts.input_files)
			{
				std::ifstream f(fn);
				if (f.fail())
				{
					std::cerr << "error: failed to open input file '" << fn
					          << "'\n";
					std::exit(EXIT_FAILURE);
				}
				compile(f, fn.c_str(), out);
			}
		}
		if (!opts.output_file.empty())
		{
			std::ofstream f(opts.output_file);
			if (f.fail())
			{
				std::cerr << "error: failed to open output file '"
				          << opts.output_file << "'\n";
				std::exit(EXIT_FAILURE);
			}
			std::string s(out.str());
			f.write(s.data(), s.size());
		}
		else
		{
			std::string s(out.str());
			std::cout.write(s.data(), s.size());
		}
		if (!opts.listing_file.empty())
		{
			std::ofstream f(opts.listing_file);
			if (f.fail())
			{
				std::cerr << "error: failed to open listing file '"
				          << opts.listing_file << "'\n";
				std::exit(EXIT_FAILURE);
			}
			InstructionList ops;
			disassemble(out, ops);
			for (auto &op : ops)
				op->dis(f);
		}
	}
	catch (SyntaxError &e)
	{
		std::cerr << "error: " << e.line() << ":" << e.column() << ": "
		          << e.what() << std::endl;
		std::exit(EXIT_FAILURE);
	}

	return 0;
}

static void print_help(Options &opts, int exit_code = EXIT_SUCCESS)
{
	std::printf(
	    "Usage: %s [options] [input files...]\n"
	    "\n"
	    "Options:\n"
	    "  -h, --help                Show this message and exit\n"
	    "  -o FILE, --output=FILE    Output file (default stdout)\n"
	    "  -L FILE, --listing=FILE   Output a listings printout\n"
	    "  input files...            Zero or more input files (default stdin)\n"
	    "\n"
	    "Written and maintained by Matthew Brush <mbrush@codebrainz.ca>\n",
	    opts.program_name.c_str());
	std::exit(exit_code);
}

static inline bool starts_with(const std::string &s, const char *p)
{
	return (s.find(p) == 0);
}

static void expand_args(StringList &args)
{
	StringList new_args;
	new_args.reserve(args.size());
	for (auto &arg : args)
	{
		auto eq_pos = arg.find('=');
		if (starts_with(arg, "--") && (eq_pos != arg.npos))
		{
			new_args.push_back(arg.substr(0, eq_pos));
			new_args.push_back(arg.substr(eq_pos + 1));
		}
		else
		{
			new_args.push_back(arg);
		}
	}
	args = new_args;
}

static void parse_args(int argc, char **argv, Options &opts)
{
	String pgm_name(argv[0]);
	auto slash = pgm_name.rfind('/');
	if (slash != pgm_name.npos)
		pgm_name = pgm_name.substr(slash + 1);
	opts.program_name = pgm_name;
	StringList args;
	args.reserve(argc - 1);
	for (int i = 1; i < argc; i++)
		args.emplace_back(argv[i]);
	expand_args(args);
	for (size_t i = 0; i < args.size(); i++)
	{
		auto &arg = args[i];
		bool last = (i == (args.size() - 1));
		if (arg == "-h" || arg == "--help")
			print_help(opts);
		else if (arg == "-o" || arg == "--output")
		{
			if (!last)
				opts.output_file = args[++i];
			else
			{
				std::cerr << "error: missing filename for --output option"
				          << std::endl;
				std::exit(EXIT_FAILURE);
			}
		}
		else if (arg == "-L" || arg == "--listing")
		{
			if (!last)
				opts.listing_file = args[++i];
			else
			{
				std::cerr << "error: missing filename for --listing option"
				          << std::endl;
				std::exit(EXIT_FAILURE);
			}
		}
		else
		{
			opts.input_files.push_back(arg);
		}
	}
}
