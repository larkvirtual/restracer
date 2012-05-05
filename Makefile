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

test: all
	$(MAKE) -C artlibgen/templates
	$(MAKE) -C regressions/features

#.PHONY: test all clean debug release
clean:
	$(MAKE) -C libs clean
	$(MAKE) -C artlibgen/src clean
	$(MAKE) -C artrepgen clean
	$(MAKE) -C regressions/features clean
	$(MAKE) -C artlibgen/templates clean

install: all
	artlibgen/src/artlibgen artlibgen/templates/posix-gcc-mt-file-lint.xml art.h art.c
	$(CC) -c art.c -g -ggdb -Wno-pointer-to-int-cast
	cp art.h /usr/local/include
	mkdir -p /usr/local/lib/art
	cp art.o /usr/local/lib/art
	cp artlibgen/src/artlibgen artrepgen/artrepgen utils/* /usr/local/bin

deinstall: uninstall

uninstall:
	rm -f /usr/local/bin/{artlibgen,artrepgen,art_make,art-gcc,art-g++,art-ld}
	rm -f /usr/local/include/art.h /usr/local/lib/art/art.o
	rm -rf /usr/local/lib/art
