-include ../Makefile.common

POP_CXXFLAGS += -I..

BIN_SOURCES = main.cpp
BIN_OBJECTS = $(BIN_SOURCES:.cpp=.o)
BIN_DEPENDS = $(BIN_SOURCES:.cpp=.d)

LIB_SOURCES = assembler.cpp ast.cpp disassembler.cpp format.cpp \
              lexer.cpp opcodes.cpp parser.cpp token.cpp
LIB_OBJECTS = $(LIB_SOURCES:.cpp=.o)
LIB_DEPENDS = $(LIB_SOURCES:.cpp=.d)

all: popvm

clean:
	$(RM) *.o *.d popvm libpop.so

popvm: $(BIN_OBJECTS) libpop.so
	$(strip $(POP_CXXLD) $(POP_CXXFLAGS) -o $@ $(BIN_OBJECTS) $(POP_LDFLAGS) -L. -lpop)

libpop.so: $(LIB_OBJECTS)
	$(strip $(POP_CXXLD) -shared $(POP_CXXFLAGS) -o $@ $(LIB_OBJECTS) $(POP_LDFLAGS))

.cpp.o:
	$(strip $(POP_CXX) $(POP_CXXFLAGS) -o $@ $<)

-include $(BIN_DEPENDS) $(LIB_DEPENDS)

.PHONY: all clean
