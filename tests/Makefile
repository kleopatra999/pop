include ../Makefile.common

POP_CXXFLAGS += -I..
POP_LDFLAGS += -L../pop -lpop

LEX_SOURCES = test_lexer.cpp
LEX_OBJECTS = $(LEX_SOURCES:.cpp=.o)
LEX_DEPENDS = $(LEX_SOURCES:.cpp=.d)

DEPENDS = $(LEX_DEPENDS)

all: test_lexer

clean:
	$(RM) *.o *.d test_lexer

test_lexer: $(LEX_OBJECTS)
	$(strip $(POP_CXXLD) $(POP_CXXFLAGS) -o $@ $(LEX_OBJECTS) $(POP_LDFLAGS))

.cpp.o:
	$(strip $(POP_CXX) $(POP_CXXFLAGS) -o $@ $<)

-include $(DEPENDS)

.PHONY: all clean
