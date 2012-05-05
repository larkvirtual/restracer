all: art-fork-g++ art-fork-gcc art-fork-ld art_fork_make
	chmod 0755 art-fork-* art_fork_make

art-fork-g++: art-g++
	sed -r 's/art\./art_fork\./g' $< > $@

art-fork-gcc: art-gcc
	sed -r 's/art\./art_fork\./g' $< > $@

art-fork-ld: art-ld
	sed -r 's/art\./art_fork\./g' $< > $@

art_fork_make: art_make
	sed -r 's/TARGET=art/TARGET=art-fork/g' $< > $@

clean:
	rm -f art-fork-g++ art-fork-gcc art-fork-ld art_fork_make
