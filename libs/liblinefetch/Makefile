CXX ?= g++
SOURCES=$(wildcard *.cc)
OBJECTS=$(SOURCES:.cc=.o)
TARGET=liblinefetch.a

all: $(SOURCES) $(TARGET) Makefile

$(TARGET): $(OBJECTS)
	$(AR) cr $@ $(OBJECTS)

.cc.o:
	$(CXX) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJECTS)
