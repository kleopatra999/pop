AC_INIT([pop], [0.1], [https://github.com/codebrainz/pop/issues])
AC_CONFIG_AUX_DIR([build/aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([pop/config.h])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AM_SILENT_RULES([yes])
AM_PROG_AR
AX_PROG_CXX14
LT_INIT
AC_DEFINE([POP_COMPILING], [1], [Defined when compiling Pop])
AC_CONFIG_FILES([
	Makefile
	pop.pc
	pop/Makefile
	tests/Makefile
])
AC_OUTPUT
