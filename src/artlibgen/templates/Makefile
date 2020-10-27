all: posix-gcc-mt-netw-lint.xml posix-gcc-mt-netw-special.xml \
posix-gcc-mt-file-fork.xml
	rm -f tmp*

clean:
	rm -f tmp* posix-gcc-mt-netw-lint.xml posix-gcc-mt-netw-special.xml \
posix-gcc-mt-file-fork.xml

posix-gcc-mt-netw-lint.xml: posix-gcc-mt-file-lint.xml
	sed -r 's/standalone/networked/g' posix-gcc-mt-file-lint.xml > tmp1
	sed -r 's/remote_mode="false"/remote_mode="true"/g' tmp1 > tmp2
	sed -r 's/trace_target="tracefile.out"/trace_target="localhost:12345"/g' \
tmp2 > tmp3
	mv tmp3 posix-gcc-mt-netw-lint.xml

posix-gcc-mt-netw-special.xml: posix-gcc-mt-file-special.xml
	sed -r 's/standalone/networked/g' posix-gcc-mt-file-special.xml > tmp4
	sed -r 's/remote_mode="false"/remote_mode="true"/g' tmp4 > tmp5
	sed -r 's/trace_target="tracefile.out"/trace_target="localhost:12345"/g' \
tmp5 > tmp6
	mv tmp6 posix-gcc-mt-netw-special.xml

posix-gcc-mt-file-fork.xml: posix-gcc-mt-file-lint.xml
	sed -r 's/fork_enabled="false"/fork_enabled="true"/g' $< > tmp7
	sed -r 's/art_start_selfinit="true"/art_start_selfinit="false"/g' tmp7 > $@
