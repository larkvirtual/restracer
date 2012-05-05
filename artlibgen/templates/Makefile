all: posix-gcc-mt-netw-lint.xml posix-gcc-mt-netw-special.xml

clean:
	rm -f tmp* posix-gcc-mt-netw-lint.xml posix-gcc-mt-netw-special.xml

posix-gcc-mt-netw-lint.xml: posix-gcc-mt-file-lint.xml
	sed -r 's/standalone/networked/g' posix-gcc-mt-file-lint.xml > tmp
	sed -r 's/remote_mode="false"/remote_mode="true"/g' tmp > tmp1
	sed -r 's/trace_target="tracefile.out"/trace_target="localhost:12345"/g' \
tmp1 > tmp2
	mv tmp2 posix-gcc-mt-netw-lint.xml
	rm -f tmp*

posix-gcc-mt-netw-special.xml: posix-gcc-mt-file-special.xml
	sed -r 's/standalone/networked/g' posix-gcc-mt-file-special.xml > tmp
	sed -r 's/remote_mode="false"/remote_mode="true"/g' tmp > tmp1
	sed -r 's/trace_target="tracefile.out"/trace_target="localhost:12345"/g' \
tmp1 > tmp2
	mv tmp2 posix-gcc-mt-netw-special.xml
