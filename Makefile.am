
SUBDIRS = CursesToolKit/lib

EXTRA_DIST = cursestoolkit.pc.in
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cursestoolkit.pc
DISTCLEANFILES = cursestoolkit.pc

.PHONY: docs

docs:
	cd docs && $(MAKE)

clean-docs:
	rm -r docs/html
	rm docs/searchdata.xml

docs-install:
	$(INSTALL) -d $(DESTDIR)$(docdir)
	cp -rvp docs/html docs/searchdata.xml $(DESTDIR)$(docdir)

docs-uninstall:
	rm -rv $(DESTDIR)$(docdir)

remake:
	$(shell ./remakesourcelist)

