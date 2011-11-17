PROGRAM=climsg
PREFIX=/usr/local

all:
	gcc -Wall main.cpp -o ./$(PROGRAM)
	strip ./$(PROGRAM)

install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $(PROGRAM) $(DESTDIR)$(PREFIX)/bin/$(PROGRAM)

uninstall:
	rm $(DESTDIR)$(PREFIX)/bin/$(PROGRAM)

clean:
	rm -f $(PROGRAM)


