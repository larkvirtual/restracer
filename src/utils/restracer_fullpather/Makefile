.PHONY: clean

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
TARGET=restracer_fullpather

all: $(SOURCES) $(TARGET) Makefile

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

install:
	mkdir -p     /usr/local/bin
	cp $(TARGET) /usr/local/bin

uninstall:
	rm -f        /usr/local/bin/$(TARGET)
