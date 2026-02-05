
SUBDIRS = CursesToolKit/lib

EXTRA_DIST = cursestoolkit.pc.in
pkgconfigdir = /usr/lib/pkgconfig
pkgconfig_DATA = cursestoolkit.pc
DISTCLEANFILES = cursestoolkit.pc


.PHONY: docs

docs:
	cd CursesToolKit/docs && $(MAKE)

clean-docs:
	rm -r CursesToolKit/docs/html
	rm CursesToolKit/docs/searchdata.xml

docs-install:
	$(INSTALL) -d $(DESTDIR)$(docdir)
	cp -rvp CursesToolKit/docs/html CursesToolKit/docs/searchdata.xml $(DESTDIR)$(docdir)

docs-uninstall:
	rm -rv $(DESTDIR)$(docdir)

