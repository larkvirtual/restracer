.PHONY: clean

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
TARGET=main

all: $(SOURCES) $(TARGET) Makefile

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
