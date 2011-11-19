PROGRAM=climsg
SOURCES=main.cpp

ifeq ($(strip $(PREFIX)),)
	PREFIX=/usr/local
endif

CXXFLAGS=-O3

$(PROGRAM):$(SOURCES)
	g++ -o $(PROGRAM) $(SOURCES) $(CXXFLAGS)

install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $(PROGRAM) $(DESTDIR)$(PREFIX)/bin/$(PROGRAM)
	strip $(DESTDIR)$(PREFIX)/bin/$(PROGRAM)

uninstall:
	rm $(DESTDIR)$(PREFIX)/bin/$(PROGRAM)

clean:
	rm -f $(PROGRAM)


