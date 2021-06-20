CXX ?= c++
SOURCES=$(wildcard *.cc)
OBJECTS=$(SOURCES:.cc=.o)
TARGET=liblinefetch.a

all: $(SOURCES) $(TARGET) Makefile

$(TARGET): $(OBJECTS)
	$(AR) cr $@ $(OBJECTS)

.cc.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJECTS)
