AM_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir)

lib_LTLIBRARIES = libpop.la

libpop_la_SOURCES = \
	assembler.cpp \
	ast.cpp \
	disassembler.cpp \
	format.cpp \
	lexer.cpp \
	opcodes.cpp \
	parser.cpp \
	token.cpp \
	value.cpp \
	vm.cpp

popincludedir = $(includedir)/pop
popinclude_HEADERS = \
	assembler.hpp \
	ast.hpp \
	codebuffer.hpp \
	compiler.hpp \
	debugvisitor.hpp \
	decoder.hpp \
	disassembler.hpp \
	error.hpp \
	format.hpp \
	instructions.hpp \
	lexer.hpp \
	location.hpp \
	opcodes.hpp \
	parser.hpp \
	pop.hpp \
	token.hpp \
	transformer.hpp \
	types.hpp \
	value.hpp \
	visitor.hpp \
	vm.hpp

bin_PROGRAMS = pop
pop_SOURCES = main.cpp
pop_LDADD = libpop.la
