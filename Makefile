PROGRAM=climsg
SOURCES=main.cpp
MANPAGE=$(PROGRAM).1

ifeq ($(strip $(PREFIX)),)
	PREFIX=/usr/local
endif

CXXFLAGS=-O3

$(PROGRAM):$(SOURCES)
	g++ -o $(PROGRAM) $(SOURCES) $(CXXFLAGS)

install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(PREFIX)/man/man1
	cp $(PROGRAM) $(DESTDIR)$(PREFIX)/bin/$(PROGRAM)
	cp $(MANPAGE) $(DESTDIR)$(PREFIX)/man/man1/$(MANPAGE)
	strip $(DESTDIR)$(PREFIX)/bin/$(PROGRAM)

uninstall:
	rm $(DESTDIR)$(PREFIX)/bin/$(PROGRAM) $(DESTDIR)$(PREFIX)/man/man1/$(MANPAGE)

clean:
	rm -f $(PROGRAM)

pkg:
	tar -cvzf $(PROGRAM).tgz $(MANPAGE) $(SOURCES) Makefile climsgexample

