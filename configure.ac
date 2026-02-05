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

m4_define([CURSESTOOLKIT_MAJ_N],[3])
m4_define([CURSESTOOLKIT_REL_N],[3])
m4_define([CURSESTOOLKIT_MIN_N],[0])
m4_define([CURSESTOOLKIT_REV_N],[1])

m4_define([ctk_api_version], [3.0])
m4_define([ctk_version],[CURSESTOOLKIT_REL_N.CURSESTOOLKIT_MIN_N.CURSESTOOLKIT_REV_N])

AC_INIT([CursesToolKit],[ctk_version],[PROJ],[CursesToolKit])

AC_CONFIG_HEADERS([config.h])
AC_PROG_CXX

AM_INIT_AUTOMAKE

AC_PROG_CC

LT_INIT([disable-static])

# libtool
CURSESTOOLKIT_MAJOR=CURSESTOOLKIT_MAJ_N()
CURSESTOOLKIT_MINOR=CURSESTOOLKIT_MIN_N()
CURSESTOOLKIT_REVISION=CURSESTOOLKIT_REV_N()
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

AC_CONFIG_MACRO_DIR([./localm4s])

NCURSES_MODULES="ncurses >= 6.1"
PKG_CHECK_MODULES(NCURSES, $NCURSES_MODULES)
AC_SUBST(NCURSES_CFLAGS)
AC_SUBST(NCURSES_LIBS)

AC_MSG_CHECKING(whether to enable source-highlight syntax highliter)
AC_ARG_ENABLE(sourcehighlight,
    [  --disable-sourcehighlight	  Don't Use source-highlight library (Default is to disable it).],
    [enable_sourcehighlight=$enableval],
    [enable_sourcehighlight="no"])
AC_MSG_RESULT($enable_sourcehighlight)

if test "$enable_sourcehighlight" = "yes"; then
SOURCEHILITE_MODULES="source-highlight >= 3.1.6"
PKG_CHECK_MODULES(SOURCEHILITE, $SOURCEHILITE_MODULES)
AC_SUBST(SOURCEHILITE_CFLAGS)
AC_SUBST(SOURCEHILITE_LIBS)
GOTSOURCEHIGHLIGHT="-D_SOURCEHIGHLIGHT_"
AC_SUBST(GOTSOURCEHIGHLIGHT)
fi

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
	IMAGEMAGICK_MODULES="Magick++ >= 7.0.0"
	PKG_CHECK_MODULES(IMAGEMAGICK,$IMAGEMAGICK_MODULES)
	AC_SUBST(IMAGEMAGICK_CFLAGS)
	AC_SUBST(IMAGEMAGICK_LIBS)
	GOTIMIAGEMAGIC="-D_IMAGEMAGICK_"
	AC_SUBST(GOTIMIAGEMAGIC)
fi

AX_BOOST_BASE([1.89],, [AC_MSG_ERROR([CursesToolKit needs Boost, but it was not found in your system])])
AX_BOOST_FILESYSTEM
AX_BOOST_REGEX

AC_CONFIG_FILES([cursestoolkit.pc])
AC_CONFIG_FILES([Makefile CursesToolKit/lib/Makefile CursesToolKit/docs/Makefile])
AC_OUTPUT

