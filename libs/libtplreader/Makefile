CXX ?= g++
CFLAGS+=$(shell pkg-config --cflags libxml++-2.6)
LDFLAGS=
SOURCES=$(wildcard *.cc)
OBJECTS=$(SOURCES:.cc=.o)
TARGET=libtplreader.a

all: $(SOURCES) $(TARGET) Makefile

$(TARGET): $(OBJECTS)
	$(AR) cr $@ $(OBJECTS)

.cc.o:
	$(CXX) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJECTS)
