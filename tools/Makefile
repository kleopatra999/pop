-include ../Makefile.common

POP_CXXFLAGS += -I..
POP_LDFLAGS += -L../pop -lpop

TK_SOURCES = tokdump.cpp
TK_OBJECTS = $(TK_SOURCES:.cpp=.o)
TK_DEPENDS = $(TK_SOURCES:.cpp=.d)

AD_SOURCES = astdump.cpp
AD_OBJECTS = $(AD_SOURCES:.cpp=.o)
AD_DEPENDS = $(AD_SOURCES:.cpp=.d)

C_SOURCES = comp.cpp
C_OBJECTS = $(C_SOURCES:.cpp=.o)
C_DEPENDS = $(C_SOURCES:.cpp=.d)

CC_SOURCES = ccomp.cpp
CC_OBJECTS = $(CC_SOURCES:.cpp=.o)
CC_DEPENDS = $(CC_SOURCES:.cpp=.d)

DIS_SOURCES = dis.cpp
DIS_OBJECTS = $(DIS_SOURCES:.cpp=.o)
DIS_DEPENDS = $(DIS_SOURCES:.cpp=.d)

DEPENDS = $(TK_DEPENDS) $(AD_DEPENDS) \
          $(C_DEPENDS) $(CC_DEPENDS) $(DIS_DEPENDS)

all: poptok popast popc popcc popdis

clean:
	$(RM) *.[do] popast popc popcc popdis

poptok: $(TK_OBJECTS) ../pop/libpop.so
	$(strip $(POP_CXXLD) $(POP_CXXFLAGS) -o $@ $(TK_OBJECTS) $(POP_LDFLAGS))

popast: $(AD_OBJECTS) ../pop/libpop.so
	$(strip $(POP_CXXLD) $(POP_CXXFLAGS) -o $@ $(AD_OBJECTS) $(POP_LDFLAGS))

popc: $(C_OBJECTS) ../pop/libpop.so
	$(strip $(POP_CXXLD) $(POP_CXXFLAGS) -o $@ $(C_OBJECTS) $(POP_LDFLAGS))

popcc: $(CC_OBJECTS) ../pop/libpop.so
	$(strip $(POP_CXXLD) $(POP_CXXFLAGS) -o $@ $(CC_OBJECTS) $(POP_LDFLAGS))

popdis: $(DIS_OBJECTS) ../pop/libpop.so
	$(strip $(POP_CXXLD) $(POP_CXXFLAGS) -o $@ $(DIS_OBJECTS) $(POP_LDFLAGS))

.cpp.o:
	$(strip $(POP_CXX) $(POP_CXXFLAGS) -o $@ $<)

../pop/libpop.so:
	$(MAKE) -C ../pop libpop.so

-include $(DEPENDS)

.PHONY: all clean
