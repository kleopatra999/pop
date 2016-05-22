AC_DEFUN([AX_PROG_CXX14], [
	AC_REQUIRE([AC_PROG_CXX])
	AC_LANG_PUSH([C++])
	CXXFLAGS="${CXXFLAGS} -std=c++14"
	AC_MSG_CHECKING([whether C++ compiler supports -std=c++14])
	AC_COMPILE_IFELSE([
		AC_LANG_PROGRAM([
#if !defined(__cplusplus) || __cplusplus < 201402L
# error "A C++14 compiler is required"
#endif
	], [])], [
		AC_MSG_RESULT([yes])
	], [
		AC_MSG_RESULT([no])
		AC_MSG_FAILURE([the C++ compiler does not support -std=c++14])
	])
	AC_LANG_POP([C++])
])
