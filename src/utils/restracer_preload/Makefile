.PHONY: clean

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
TARGET=restracer_preload.so

all: $(SOURCES) $(TARGET) Makefile

$(TARGET): $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS) -ldl -rdynamic

# without -ldl segfaults
# is -DPIC really needed?
# -fvisibility=hidden

.c.o:
#	$(CC) -c -O1 -fPIC -DPIC $(CFLAGS) $< -o $@
	$(CC) -c -O1 -fPIC -DPIC           $< -o $@
# we must not to change optimization level, so skipping $(CFLAGS)

clean:
	rm -f $(TARGET) $(OBJECTS)

install:
	mkdir -p     /usr/local/lib/restracer/preload/
	cp $(TARGET) /usr/local/lib/restracer/preload/

uninstall:
	rm -rf       /usr/local/lib/restracer/preload/
