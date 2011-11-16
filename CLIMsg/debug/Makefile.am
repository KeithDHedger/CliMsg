

include ../flagsandlibs
include ../sources

climsgdebug_SOURCES = $(COMMONSRC)

climsgdebug_CFLAGS = -ggdb -O0

bin_PROGRAMS = climsgdebug

install-exec-local:
	cd $(DESTDIR)$(bindir) && \
	rm climsgdebug

