
AUTOMAKE_OPTIONS = subdir-objects

include ../flagsandlibs
include ../sources

lib_LTLIBRARIES = libcursestoolkit.la
libcursestoolkit_la_SOURCES = $(COMMONSRC)

#man1_MANS = ../resources/man/cursesmenus.1


libcursestoolkit_la_LDFLAGS = $(AM_LDFLAGS) -version-info $(CURSESTOOLKIT_VERSION)

ctktoolkit_includedir = $(includedir)/cursestoolkit
ctktoolkit_include_HEADERS = ../src/*.h
