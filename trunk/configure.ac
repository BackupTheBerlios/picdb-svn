dnl initialise autoconf and automake
AC_INIT(picdb, 0.0.1, http://l33th4x0r.dyndns.org/)
AC_PREREQ(2.57)
AC_CONFIG_SRCDIR(include/picdb.hpp)
AM_INIT_AUTOMAKE(picdb, 0.0.1)
AC_PREFIX_DEFAULT(${HOME})

dnl check for programs
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL

dnl check for headers/libraries
AC_HEADER_STDC
AC_C_CONST
AC_CHECK_FUNCS(strdup)
AC_CHECK_LIB(dl, dlopen,CXXFLAGS="$CXXFLAGS -ldl")
AC_CHECK_LIB(m, pow,, AC_MSG_ERROR([libm was not found.]))
AC_CHECK_LIB(z, gzopen,, AC_MSG_ERROR([zlib was not found.]))
AC_CHECK_LIB(IL, ilInit,,AC_MSG_ERROR([
****************************************************************
* You do not have DevIL installed.  This is required to build. *
* You may find it at http://openil.sourceforge.net/.           *
****************************************************************]))
AC_CHECK_LIB(ILU, iluFlipImage,, AC_MSG_ERROR([libILU was not found.]))
AC_CHECK_LIB([xerces-c], [main],, AC_MSG_ERROR([cannot find xerces library]), -lpthread)


#CFLAGS="-I/usr/local/include -g -I`pwd`/include -fPIC -Wall `pcre-config --cflags` -DGNU_SOURCE"
#CXXFLAGS="$CFLAGS"
#LIBS="-L/usr/local/lib -lm -ldl -lIL -lILU `pcre-config --libs` -lxerces-c"
CFLAGS="$CFLAGS -I`pwd`/include -Wall `pcre-config --cflags` -DGNU_SOURCE "
CXXFLAGS="$CXXFLAGS -I`pwd`/include -Wall `pcre-config --cflags` -DGNU_SOURCE "
LIBS="-L/usr/local/lib `pcre-config --libs` -lxerces-c -lIL -lILU"

AC_CONFIG_FILES([Makefile src/Makefile backends/Makefile backends/src/Makefile frontends/Makefile frontends/text/Makefile frontends/text/src/Makefile])
AM_CONFIG_HEADER([config.h])
AC_OUTPUT
