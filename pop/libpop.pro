TEMPLATE = lib
TARGET = pop
CONFIG -= qt app_bundle
QMAKE_CXXFLAGS = -std=c++14 -Wno-unused-parameter
INCLUDEPATH += $$PWD/..
DEFINES += POP_COMPILING=1

SOURCES = \
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

HEADERS = \
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
