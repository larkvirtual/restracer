#!/bin/sh

LIBXMLNAME=$( find /usr/lib* -maxdepth 1 -type l,f -name libxml++-\*.so | head -n1 | sed 's@.so.*$@@' | xargs basename )
LIBXMLOPTS=$( pkg-config --cflags --libs $LIBXMLNAME )

# -Wno-deprecated-declarations prevents libxml++-2.6 warnings

g++ -O3 -pipe -Isrc/libs/liblinefetch -Isrc/libs/libtplreader \
    -Wno-deprecated-declarations \
    $LIBXMLOPTS \
    src/artlibgen/src/*.cc src/libs/liblinefetch/*.cc src/libs/libtplreader/*.cc \
    -o src/artlibgen/src/artlibgen

g++ -O3 -pipe -Isrc/libs/liblinefetch -Isrc/libs/libtplreader \
    -Wno-deprecated-declarations \
    $LIBXMLOPTS \
    src/artrepgen/*.cc src/libs/liblinefetch/*.cc src/libs/libtplreader/*.cc \
    -o src/artrepgen/artrepgen
