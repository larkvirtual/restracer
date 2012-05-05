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
	cp art.h art_fork.h /usr/local/include
	mkdir -p /usr/local/lib/art
	cp art.o art_fork.o /usr/local/lib/art
	cp artlibgen/src/artlibgen artrepgen/artrepgen utils/art* /usr/local/bin

deinstall: uninstall

uninstall:
	rm -f /usr/local/bin/artlibgen /usr/local/bin/artrepgen \
/usr/local/bin/art_make /usr/local/bin/art-gcc /usr/local/bin/art-g++ \
/usr/local/bin/art-ld /usr/local/bin/art_fork_make /usr/local/bin/art-fork-gcc \
/usr/local/bin/art-fork-g++ /usr/local/bin/art-fork-ld
	rm -f /usr/local/include/art.h /usr/local/lib/art/art.o
	rm -f /usr/local/include/art_fork.h /usr/local/lib/art/art_fork.o
	rm -rf /usr/local/lib/art
