-include ../Makefile.common

POP_CXXFLAGS += -I..

LIB_SOURCES = assembler.cpp ast.cpp disassembler.cpp format.cpp \
              lexer.cpp opcodes.cpp parser.cpp token.cpp
LIB_OBJECTS = $(LIB_SOURCES:.cpp=.o)
LIB_DEPENDS = $(LIB_SOURCES:.cpp=.d)

all: libpop.so libpop.a

clean:
	$(RM) *.[aod] *.so

libpop.so: $(LIB_OBJECTS)
	$(strip $(POP_CXXLD) -shared $(POP_CXXFLAGS) -o $@ $(LIB_OBJECTS) $(POP_LDFLAGS))

libpop.a: $(LIB_OBJECTS)
	$(strip $(POP_AR) $@ $(LIB_OBJECTS))

.cpp.o:
	$(strip $(POP_CXX) $(POP_CXXFLAGS) -o $@ $<)

-include $(LIB_DEPENDS)

.PHONY: all clean
