// vm.cpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifdef HAVE_CONFIG_H
#include <pop/config.h>
#endif

#include <pop/pop.hpp>
#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include <cassert>

#define str_eq(s1, s2) (std::strcmp(s1, s2) == 0)
#define str_eqor(s1, s2, s3) (str_eq(s1, s2) || str_eq(s1, s3))

struct CmdOptions
{
	std::string program;
	std::vector<std::string> rest_args;
	std::vector<std::string> input_files;
	std::string output_file;
	bool do_astdump;
	bool do_compile;
	bool do_disasm;
	bool do_listing;
	bool do_tokens;

	CmdOptions(int argc, char **argv)
	    : program((argc > 0) ? argv[0] : "popvm"), output_file("-"),
	      do_astdump(false), do_compile(false), do_disasm(false),
	      do_listing(false), do_tokens(false)
	{
		auto slash = program.rfind('/');
		if (slash != program.npos)
			program = program.substr(slash + 1);

		for (int i = 1; i < argc; i++)
		{
			if (str_eqor(argv[i], "-h", "--help"))
				print_help();
			else if (str_eqor(argv[i], "-a", "--astdump"))
				do_astdump = true;
			else if (str_eqor(argv[i], "-c", "--compile"))
				do_compile = true;
			else if (str_eqor(argv[i], "-d", "--disasm"))
				do_disasm = true;
			else if (str_eqor(argv[i], "-l", "--listing"))
				do_listing = true;
			else if (str_eqor(argv[i], "-t", "--tokens"))
				do_tokens = true;
			else if (str_eqor(argv[i], "-o", "--output"))
			{
				if (i < (argc - 1))
					output_file = argv[++i];
				else
				{
					print_error("missing filename argument for -o option");
				}
			}
			else if (str_eq(argv[i], "--"))
			{
				for (int j = i + 1; j < argc; j++)
					rest_args.push_back(argv[j]);
				i = argc;
			}
			else
			{
				input_files.push_back(argv[i]);
			}
		}
		unsigned int cnt(0);
		if (do_astdump)
			cnt++;
		if (do_compile)
			cnt++;
		if (do_disasm)
			cnt++;
		if (do_listing)
			cnt++;
		if (do_tokens)
			cnt++;
		if (cnt > 1)
		{
			print_error(
			    "the -a, -c, -d, -l, and -t options are mutually exclusive");
		}
	}

#ifndef NDEBUG
	void debug(std::ostream &out) const
	{
		out << "Options:\n"
		    << "  Program: " << program << "\n"
		    << "  Output File: " << output_file << "\n"
		    << "  Do AST Dump: " << (do_astdump ? "true" : "false") << "\n"
		    << "  Do Compile: " << (do_compile ? "true" : "false") << "\n"
		    << "  Do Disasm: " << (do_disasm ? "true" : "false") << "\n"
		    << "  Do Listing: " << (do_listing ? "true" : "false") << "\n"
		    << "  Do Tokens: " << (do_tokens ? "true" : "false") << "\n"
		    << "  Input Files:\n";
		for (auto &f : input_files)
			out << "    " << f << "\n";
		out << "  Rest Arguments:\n";
		for (auto &a : rest_args)
			out << "    " << a << "\n";
	}
#endif

	void print_error(const char *fmt, ...)
	{
		std::fprintf(stderr, "%s: error: ", program.c_str());
		va_list ap;
		va_start(ap, fmt);
		std::vfprintf(stderr, fmt, ap);
		va_end(ap);
		std::fputc('\n', stderr);
		std::exit(EXIT_FAILURE);
	}

	void print_help()
	{
		std::printf(
		    "Usage: %s [options] [input files...] [-- args...]\n"
		    "\n"
		    "Command-line interface to the Pop programming language\n"
		    "compiler and runtime environment.\n"
		    "\n"
		    "Options:\n"
		    "  -h, --help      show this message and exit\n"
		    "  -a, --astdump   pretty-print the parsed AST and exit\n"
		    "  -c, --compile   just compile bytecode, don't interpret\n"
		    "  -d, --disasm    pretty-print a disassembly listing and exit\n"
		    "  -l, --listing   pretty-print an instruction listing and exit\n"
		    "  -t, --tokens    pretty-print lexical tokens and exit\n"
		    "  -o, --output    for -a, -c, -d, -l, -t, file to print to\n"
		    "  input files...  program to execute or empty for REPL\n"
		    "                      a .pop file is first compiled\n"
		    "                      a .pbc files is directly interpreted\n"
		    "  -- args...      arguments forwarded to program being run\n"
		    "\n"
		    "If there are .pop and .pbc files in the same directory, and\n"
		    "the .pbc file is newer than the .pop file, it is executed\n"
		    "directly, otherwise the .pop file is compiled into a .pbc\n"
		    "bytecode file and saved in the same directory as the .pop\n"
		    "file, before it is executed.\n"
		    "\n"
		    "If an -- is encountered in the arguments, all of the rest\n"
		    "of the arguments are collected and used as the argument\n"
		    "vector for the program being run.\n"
		    "\n"
		    "Written and maintained by Matthew Brush <mbrush@codebrainz.ca>\n",
		    program.c_str());
	}
};

static void print_ast(CmdOptions &opts)
{
	std::ofstream ofile;
	bool use_stdout = true;

	if (opts.output_file != "-")
	{
		use_stdout = false;
		ofile.open(opts.output_file);
		if (!ofile)
		{
			opts.print_error("failed to open output file '%s': %s (%d)",
			                 opts.output_file.c_str(), std::strerror(errno),
			                 errno);
		}
	}

	Pop::DebugVisitor visitor(use_stdout ? std::cout : ofile);
	if (opts.input_files.empty())
	{
		auto mod = Pop::parse(std::cin, "<stdin>");
		mod->accept(visitor);
		if (std::cin.fail() && !std::cin.eof())
		{
			opts.print_error("error reading standard input: %s (%d)",
			                 std::strerror(errno), errno);
		}
	}
	else
	{
		for (auto &in_file : opts.input_files)
		{
			std::ifstream ifile(in_file);
			if (!ifile)
			{
				opts.print_error("failed to open input file '%s': %s (%d)",
				                 in_file.c_str(), std::strerror(errno), errno);
			}
			auto mod = Pop::parse(ifile, in_file.c_str());
			mod->accept(visitor);
			if (ifile.fail() && !ifile.eof())
			{
				opts.print_error("error reading input file '%s': %s (%d)",
				                 in_file.c_str(), std::strerror(errno), errno);
			}
		}
	}

	if (ofile.fail())
	{
		opts.print_error("error writing output file '%s': %s (%d)",
		                 opts.output_file.c_str(), std::strerror(errno), errno);
	}

	if (use_stdout)
		std::cout.flush();
	else
		ofile.close();

	std::exit(EXIT_SUCCESS);
}

static void compile_bytecode(CmdOptions &opts)
{
	std::ofstream ofile;
	bool use_stdout = false;
	if (opts.output_file != "-")
	{
		ofile.open(opts.output_file);
		if (!ofile)
		{
			opts.print_error("failed to open output file '%s': %s (%d)",
			                 opts.output_file.c_str(), std::strerror(errno),
			                 errno);
		}
	}
	else
	{
		use_stdout = true;
	}

	if (opts.input_files.empty())
	{
		Pop::compile(std::cin, "<stdin>", use_stdout ? std::cout : ofile);
		if (std::cin.fail() && !std::cin.eof())
		{
			opts.print_error("error reading standard input: %s (%d)",
			                 std::strerror(errno), errno);
		}
	}
	else
	{
		for (auto &in_file : opts.input_files)
		{
			std::ifstream ifile(in_file);
			if (!ifile)
			{
				opts.print_error("failed to open input file '%s': %s (%d)",
				                 in_file.c_str(), std::strerror(errno), errno);
			}
			Pop::compile(ifile, in_file, use_stdout ? std::cout : ofile);
			if (ifile.fail() && !ifile.eof())
			{
				opts.print_error("error reading input file '%s': %s (%d)",
				                 in_file.c_str(), std::strerror(errno), errno);
			}
		}
	}

	if (ofile.fail())
	{
		opts.print_error("error writing output file '%s': %s (%d)",
		                 opts.output_file, std::strerror(errno), errno);
	}

	if (use_stdout)
		std::cout.flush();
	else
		ofile.close();

	std::exit(EXIT_SUCCESS);
}

static void print_disassembly(CmdOptions &opts)
{
	std::ofstream ofile;
	bool use_stdout = false;
	if (opts.output_file != "-")
	{
		ofile.open(opts.output_file);
		if (!ofile)
		{
			opts.print_error("failed to open output file '%s': %s (%d)",
			                 opts.output_file.c_str(), std::strerror(errno),
			                 errno);
		}
	}
	else
	{
		use_stdout = true;
	}

	Pop::InstructionList ops;
	std::stringstream sout;
	if (opts.input_files.empty())
	{
		Pop::compile(std::cin, "<stdin>", sout);
		if (std::cin.fail() && !std::cin.eof())
		{
			opts.print_error("error reading standard input: %s (%d)",
			                 std::strerror(errno), errno);
		}
	}
	else
	{
		for (auto &in_file : opts.input_files)
		{
			std::ifstream ifile(in_file);
			if (!ifile)
			{
				opts.print_error("failed to open input file '%s': %s (%d)",
				                 in_file.c_str(), std::strerror(errno), errno);
			}
			Pop::compile(ifile, in_file, sout);
			if (ifile.fail() && !ifile.eof())
			{
				opts.print_error("error reading input file '%s': %s (%d)",
				                 in_file.c_str(), std::strerror(errno), errno);
			}
		}
	}

	Pop::disassemble(sout, ops);
	for (auto &op : ops)
		op->dis(use_stdout ? std::cout : ofile);

	if (ofile.fail())
	{
		opts.print_error("error writing output file '%s': %s (%d)",
		                 opts.output_file, std::strerror(errno), errno);
	}

	if (use_stdout)
		std::cout.flush();
	else
		ofile.close();

	std::exit(EXIT_SUCCESS);
}

static void print_listing(CmdOptions &opts)
{
	std::ofstream ofile;
	bool use_stdout = false;
	if (opts.output_file != "-")
	{
		ofile.open(opts.output_file);
		if (!ofile)
		{
			opts.print_error("failed to open output file '%s': %s (%d)",
			                 opts.output_file.c_str(), std::strerror(errno),
			                 errno);
		}
	}
	else
	{
		use_stdout = true;
	}

	std::vector<Pop::ModulePtr> modules;
	if (opts.input_files.empty())
	{
		modules.emplace_back(Pop::parse(std::cin, "<stdin>"));
		if (std::cin.fail() && !std::cin.eof())
		{
			opts.print_error("error reading standard input: %s (%d)",
			                 std::strerror(errno), errno);
		}
	}
	else
	{
		for (auto &in_file : opts.input_files)
		{
			std::ifstream ifile(in_file);
			if (!ifile)
			{
				opts.print_error("failed to open input file '%s': %s (%d)",
				                 in_file.c_str(), std::strerror(errno), errno);
			}
			modules.emplace_back(Pop::parse(ifile, in_file.c_str()));
			if (ifile.fail() && !ifile.eof())
			{
				opts.print_error("error reading input file '%s': %s (%d)",
				                 in_file.c_str(), std::strerror(errno), errno);
			}
		}
	}

	for (auto &mod : modules)
	{
		for (auto &op : Pop::transform(mod))
			op->list(use_stdout ? std::cout : ofile);
	}

	if (ofile.fail())
	{
		opts.print_error("error writing output file '%s': %s (%d)",
		                 opts.output_file, std::strerror(errno), errno);
	}

	if (use_stdout)
		std::cout.flush();
	else
		ofile.close();

	std::exit(EXIT_SUCCESS);
}

static void print_tokens(CmdOptions &opts)
{
	std::ofstream ofile;
	bool use_stdout = false;

	if (opts.output_file != "-")
	{
		ofile.open(opts.output_file);
		if (!ofile)
		{
			opts.print_error("error opening output file '%s': %s (%d)",
			                 opts.output_file.c_str(), std::strerror(errno),
			                 errno);
		}
	}
	else
	{
		use_stdout = true;
	}

	std::vector<Pop::Token> tokens;
	if (opts.input_files.empty())
	{
		Pop::Token tok;
		Pop::Lexer lex(std::cin, "<stdin>");
		while (lex.next_token(tok) != Pop::TokenKind::END)
			tokens.emplace_back(tok);
		if (std::cin.fail() && !std::cin.eof())
		{
			opts.print_error("error reading standard input: %s (%d)",
			                 std::strerror(errno), errno);
		}
	}
	else
	{
		for (auto &in_file : opts.input_files)
		{
			std::ifstream ifile(in_file);
			if (!ifile)
			{
				opts.print_error("error opening input file '%s': %s (%d)",
				                 in_file.c_str(), std::strerror(errno), errno);
			}
			Pop::Token tok;
			Pop::Lexer lex(ifile, in_file.c_str());
			while (lex.next_token(tok) != Pop::TokenKind::END)
				tokens.emplace_back(tok);
			if (ifile.fail() && !ifile.eof())
			{
				opts.print_error("error reading input file '%s': %s (%d)",
				                 in_file.c_str(), std::strerror(errno), errno);
			}
		}
	}

	for (auto &tok : tokens)
		tok.debug(use_stdout ? std::cout : ofile);

	if (ofile.fail())
	{
		opts.print_error("error writing output file '%s': %s (%d)",
		                 opts.output_file.c_str(), std::strerror(errno), errno);
	}

	if (use_stdout)
		std::cout.flush();
	else
		ofile.close();

	std::exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	CmdOptions opts(argc, argv);

#ifndef NDEBUG
// opts.debug(std::cout);
#endif

	if (opts.do_astdump)
		print_ast(opts);
	else if (opts.do_compile)
		compile_bytecode(opts);
	else if (opts.do_disasm)
		print_disassembly(opts);
	else if (opts.do_listing)
		print_listing(opts);
	else if (opts.do_tokens)
		print_tokens(opts);
	else
	{
		// run VM
	}

	return 0;
}
