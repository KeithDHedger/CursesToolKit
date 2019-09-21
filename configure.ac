#
#

#shared library versioning
#LIBRARY_VERSION=4:1:0
#               | | |
#        +------+ | +---+
#        |        |     |
#      current:revision:age
#        |        |     |
#        |        |     +- increment if interfaces have been added
#        |        |        set to zero if interfaces have been removed
#        |        |        or changed
#        |        +- increment if source code has changed
#        |           set to zero if current is incremented
#        +- increment if interfaces have been added, removed or changed

m4_define([CURSESTOOLKIT_MAJ_N],[2])
m4_define([CURSESTOOLKIT_REL_N],[1])
m4_define([CURSESTOOLKIT_MIN_N],[1])
m4_define([CURSESTOOLKIT_REV_N],[2])

m4_define([ctk_api_version], [1.0])
m4_define([ctk_version],[CURSESTOOLKIT_REL_N.CURSESTOOLKIT_MIN_N.CURSESTOOLKIT_REV_N])

AC_INIT([CursesToolKit],[ctk_version],[PROJ],[CursesToolKit])

AC_CONFIG_HEADER([config.h])
AC_PROG_CXX

AM_INIT_AUTOMAKE

AC_PROG_CC
AC_HEADER_STDC

LT_INIT([disable-static])

# libtool
CURSESTOOLKIT_MAJOR=CURSESTOOLKIT_MAJ_N()
CURSESTOOLKIT_MINOR=CURSESTOOLKIT_MIN_N()
CURSESTOOLKIT_REVISION=CURSESTOOLKIT_REV_N()
#CURSESTOOLKIT_INTERFACE=$(( CURSESTOOLKIT_MAJ_N() ))
CURSESTOOLKIT_INTERFACE=$(( CURSESTOOLKIT_MAJ_N() ))

CURSESTOOLKIT_LIBTOOL_VERSION=$CURSESTOOLKIT_INTERFACE:$CURSESTOOLKIT_REVISION:$CURSESTOOLKIT_MINOR
CURSESTOOLKIT_VERSION=$CURSESTOOLKIT_REL_N:$CURSESTOOLKIT_REVISION:$CURSESTOOLKIT_MINOR

AC_SUBST(CURSESTOOLKIT_VERSION)
AC_SUBST(CURSESTOOLKIT_LIBTOOL_VERSION)

AC_SUBST(CFLAGS)
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)

CFLAGS=-Wall
AC_SUBST(CFLAGS)

TERMKEY_MODULES="termkey >= 0.2"
PKG_CHECK_MODULES(TERMKEY, $TERMKEY_MODULES)
AC_SUBST(TERMKEY_CFLAGS)
AC_SUBST(TERMKEY_LIBS)

SOURCEHILITE_MODULES="source-highlight >= 3.1.6"
PKG_CHECK_MODULES(SOURCEHILITE, $SOURCEHILITE_MODULES)
AC_SUBST(SOURCEHILITE_CFLAGS)
AC_SUBST(SOURCEHILITE_LIBS)

AC_CHECK_PROGS([DOXYGEN], [doxygen])
if test -z "$DOXYGEN";
   then AC_MSG_WARN([Doxygen not found - continuing without Doxygen support])
fi
AM_CONDITIONAL([HAVE_DOXYGEN],
[test -n "$DOXYGEN"])AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([CursesToolKit/docs/Doxyfile])])

AC_MSG_CHECKING(whether to enable Imagemagick graphics)
AC_ARG_ENABLE(imagemagick,
    [  --disable-imagemagick	  Don't Use Imagemagick (Default is to enable).],
    [enable_imagemagick=$enableval],
    [enable_imagemagick="yes"])
AC_MSG_RESULT($enable_imagemagick)

if test "$enable_imagemagick" = "yes"; then
	AC_DEFINE([_IMAGEMAGICK_], [], [Use IMAGEMAGICK])
	IMAGEMAGICK_MODULES="Magick++ >= 7.0.0"
	PKG_CHECK_MODULES(IMAGEMAGICK,$IMAGEMAGICK_MODULES)
	AC_SUBST(IMAGEMAGICK_CFLAGS)
	AC_SUBST(IMAGEMAGICK_LIBS)
fi

AC_CONFIG_FILES([cursestoolkit.pc])
AC_OUTPUT(Makefile CursesToolKit/lib/Makefile CursesToolKit/docs/Makefile)

