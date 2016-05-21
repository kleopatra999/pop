-include ../Makefile.common

POP_CXXFLAGS += -I..

LIB_SOURCES = assembler.cpp ast.cpp disassembler.cpp format.cpp \
              lexer.cpp opcodes.cpp parser.cpp token.cpp
LIB_OBJECTS = $(LIB_SOURCES:.cpp=.o)
LIB_DEPENDS = $(LIB_SOURCES:.cpp=.d)

CC_SOURCES = main.cpp
CC_OBJECTS = $(CC_SOURCES:.cpp=.o)
CC_DEPENDS = $(CC_SOURCES:.cpp=.d)

NC_SOURCES = nativec.cpp
NC_OBJECTS = $(NC_SOURCES:.cpp=.o)
NC_DEPENDS = $(NC_SOURCES:.cpp=.d)

DEPENDS = $(LIB_DEPENDS) $(CC_DEPENDS) $(NC_DEPENDS)

all: popc popnc

clean:
	$(RM) *.o *.d popc popnc libpop.so

popc: $(CC_OBJECTS) libpop.so
	$(strip $(POP_CXXLD) $(POP_CXXFLAGS) -o $@ $(CC_OBJECTS) $(POP_LDFLAGS) -L. -lpop)

popnc: $(NC_OBJECTS) libpop.so
	$(strip $(POP_CXXLD) $(POP_CXXFLAGS) -o $@ $(NC_OBJECTS) $(POP_LDFLAGS) -L. -lpop)

libpop.so: $(LIB_OBJECTS)
	$(strip $(POP_CXXLD) -shared $(POP_CXXFLAGS) -o $@ $(LIB_OBJECTS) $(POP_LDFLAGS))

.cpp.o:
	$(strip $(POP_CXX) $(POP_CXXFLAGS) -o $@ $<)

-include $(DEPENDS)

.PHONY: all clean
