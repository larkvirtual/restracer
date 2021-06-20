.PHONY: clean

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
TARGET=main

all: $(SOURCES) $(TARGET) Makefile

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

main.o: main.c
	$(CC) -c $(CFLAGS) $< -o $@

foo.o: foo.c
	cc -c $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
