AM_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir) -UPOP_COMPILING
LDADD = $(top_builddir)/pop/libpop.la

TESTS = test_lexer
check_PROGRAMS = $(TESTS)

test_lexer_SOURCES = test_lexer.cpp

EXTRA_DIST = fib.pop
