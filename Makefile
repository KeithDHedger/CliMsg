PROGRAM=climsg
PREFIX=/usr/local

all:
	gcc -Wall main.cpp -o ./$(PROGRAM)
	
#
#install:
#	makeswitch -c $(PROGRAM).conf
#	mkdir -p $(DESTDIR)$(PREFIX)/bin
#	cp $(PROGRAM) $(DESTDIR)$(PREFIX)/bin/$(PROGRAM)
#
#uninstall:
#	rm $(DESTDIR)/$(PREFIX)/bin/$(PROGRAM)
#
clean:
	rm -f $(PROGRAM)


