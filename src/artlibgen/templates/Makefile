SED:=$(shell type gsed > /dev/null && echo gsed || echo sed)

all: compiled

sources: sources-generics sources-specials

sources-generics: templates
	@for i in posix*lint.xml posix*fork.xml; do \
d=`echo "$$i" | ${SED} 's/.xml//g' -`; \
rm -rf "$$d"; mkdir "$$d";\
../src/artlibgen "$$i" "$$d/art.h" "$$d/art.c"; \
done

sources-specials: templates
	@for i in posix*special.xml; do \
d=`echo "$$i" | ${SED} 's/.xml//g' -`; \
rm -rf "$$d"; mkdir "$$d";\
../src/artlibgen "$$i" "$$d/art.h" "$$d/art.c"; \
cp f04-api.h "$$d"; \
done

compiled-generics: sources
	@for i in posix-gcc-mt-file-fork.xml posix-gcc-mt-file-lint.xml posix-gcc-mt-netw-lint.xml; do \
d=`echo "$$i" | ${SED} 's/.xml//g' -`; \
cc -c "$$d/art.c" -I "$$d" -o "$$d/art.o" \
-O2 -g -ggdb -Wno-pointer-to-int-cast -Wno-int-conversion; \
done

compiled-specials: sources
	@for i in posix*special.xml; do \
d=`echo "$$i" | ${SED} 's/.xml//g' -`; \
cc -c "$$d/art.c" -I "$$d" -I. -o "$$d/art.o" \
-O2 -g -ggdb -Wno-pointer-to-int-cast -Wno-int-conversion; \
cc -c f04-api.c            -I. -o "$$d/f04-api.o" \
-O2 -g -ggdb; \
done

compiled: compiled-generics compiled-specials

templates: posix-gcc-mt-netw-lint.xml posix-gcc-mt-netw-special.xml \
posix-gcc-mt-file-fork.xml

clean:
	@for i in posix*.xml; do \
d=`echo "$$i" | ${SED} 's/.xml//g' -`; \
rm -rf "$$d"; \
done
	@rm -f tmp* posix-gcc-mt-netw-lint.xml posix-gcc-mt-netw-special.xml \
posix-gcc-mt-file-fork.xml

posix-gcc-mt-netw-lint.xml: posix-gcc-mt-file-lint.xml
	@${SED} -r 's/standalone/networked/g' posix-gcc-mt-file-lint.xml > $@
	@${SED} -i -r 's/remote_mode="false"/remote_mode="true"/g'         $@
	@${SED} -i -r 's/trace_target="tracefile.out"/trace_target="localhost:12345"/g' $@

posix-gcc-mt-netw-special.xml: posix-gcc-mt-file-special.xml
	@${SED} -r 's/standalone/networked/g' posix-gcc-mt-file-special.xml > $@
	@${SED} -i -r 's/remote_mode="false"/remote_mode="true"/g'            $@
	@${SED} -i -r 's/trace_target="tracefile.out"/trace_target="localhost:12345"/g' $@

posix-gcc-mt-file-fork.xml: posix-gcc-mt-file-lint.xml
	@${SED} -r 's/fork_enabled="false"/fork_enabled="true"/g' $<           > $@
	@${SED} -i -r 's/art_start_selfinit="true"/art_start_selfinit="false"/g' $@
