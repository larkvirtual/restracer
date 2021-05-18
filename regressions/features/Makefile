all: tracefile-method networked-method

tracefile-method:
	sh ./build-n-run-tracefile.sh

networked-method: build-n-run-networked.sh usleep
	sh ./build-n-run-networked.sh
	@echo "*******************************************************************"
	@echo "* At the end you have to see the same number of passed test as for"
	@echo "* tracing via tracefile. Else (on slow machine) tune"
	@echo "* argument for ./usleep in build-n-run-networked.sh script."
	@echo "* For example, try to set ./usleep argument into 32 or 64 value."
	@echo "*******************************************************************"

build-n-run-networked.sh:
	cp build-n-run-tracefile.sh build-n-run-networked.sh
	patch -s -i netw.diff

usleep: usleep.c
	$(CC) -o usleep usleep.c

clean: clean-dirs
	@sh ./clean.sh
	@rm -f usleep build-n-run-networked.sh

clean-dirs:
	@for i in `find -type d | tail -n +2`; do \
rm -f $$i/.tracefile.out; \
rm -f $$i/main; \
rm -f $$i/main.o; \
rm -f $$i/template.tmp; \
done
