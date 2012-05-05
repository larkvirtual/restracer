all: tracefile-method networked-method

tracefile-method:
	sh ./build-n-run-tracefile.sh

networked-method: build-n-run-networked.sh usleep
	@echo "*******************************************************************"
	@echo "* Now I will run network connection testing for client and server."
	@echo "* At the end you have to see the same number of passed test as for"
	@echo "* tracing via tracefile. Else (on too slow or too fast machine) tune"
	@echo "* argument for ./usleep in build-n-run-networked.sh script."
	@echo "* Press ENTER when ready..."
	@echo "*******************************************************************"
	@read line
	sh ./build-n-run-networked.sh

build-n-run-networked.sh:
	cp build-n-run-tracefile.sh build-n-run-networked.sh
	patch < netw.diff

usleep: usleep.c
	$(CC) -o usleep usleep.c

clean:
	sh ./clean.sh
	rm -f usleep build-n-run-networked.sh
