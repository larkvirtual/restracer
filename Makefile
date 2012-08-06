DESTDIR?=/usr/local

debug:
	CFLAGS="-g -ggdb -Wall -Wextra -Werror -ansi -DART_DEBUG_INSERT_DEVEL_COMMENT \
-DART_DEBUG" $(MAKE) all
#	CFLAGS="-Wall -Wextra -Werror -ansi -pedantic -std=c++0x \
#-DART_DEBUG_INSERT_DEVEL_COMMENT \
#-DART_DEBUG" make all

release:
	CFLAGS="-O3 -fomit-frame-pointer" $(MAKE) all

all:
	$(MAKE) -C libs
	$(MAKE) -C artlibgen/src
	$(MAKE) -C artrepgen
	$(MAKE) -C artlibgen/templates
	$(MAKE) -C utils

test: all
	$(MAKE) -C artlibgen/templates
	$(MAKE) -C regressions/features

clean:
	$(MAKE) -C libs clean
	$(MAKE) -C artlibgen/src clean
	$(MAKE) -C artrepgen clean
	$(MAKE) -C regressions/features clean
	$(MAKE) -C artlibgen/templates clean
	$(MAKE) -C utils clean
	rm -f art.[cho] art_fork.[cho]

install: all
	artlibgen/src/artlibgen artlibgen/templates/posix-gcc-mt-file-lint.xml art.h art.c
	artlibgen/src/artlibgen artlibgen/templates/posix-gcc-mt-file-fork.xml art_fork.h art_fork.c
	$(CC) -c art.c art_fork.c -g -ggdb -Wno-pointer-to-int-cast
	cp art.h art_fork.h $(DESTDIR)/include
	mkdir -p $(DESTDIR)/lib/art
	cp art.o art_fork.o $(DESTDIR)/lib/art
	cp artlibgen/src/artlibgen artrepgen/artrepgen utils/art* $(DESTDIR)/bin

deinstall: uninstall

uninstall:
	rm -f $(DESTDIR)/bin/artlibgen $(DESTDIR)/bin/artrepgen \
$(DESTDIR)/bin/art_make $(DESTDIR)/bin/art-gcc $(DESTDIR)/bin/art-g++ \
$(DESTDIR)/bin/art-ld $(DESTDIR)/bin/art_fork_make $(DESTDIR)/bin/art-fork-gcc \
$(DESTDIR)/bin/art-fork-g++ $(DESTDIR)/bin/art-fork-ld
	rm -f $(DESTDIR)/include/art.h $(DESTDIR)/lib/art/art.o
	rm -f $(DESTDIR)/include/art_fork.h $(DESTDIR)/lib/art/art_fork.o
	rm -rf $(DESTDIR)/lib/art
