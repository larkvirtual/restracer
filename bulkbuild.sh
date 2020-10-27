#!/bin/sh

# -Wno-deprecated-declarations prevents libxml++-2.6 warnings

g++ -O3 -pipe -Isrc/libs/liblinefetch -Isrc/libs/libtplreader \
    -Wno-deprecated-declarations \
    `pkg-config --cflags --libs libxml++-2.6` \
    src/artlibgen/src/*.cc src/libs/liblinefetch/*.cc src/libs/libtplreader/*.cc \
    -o src/artlibgen/src/artlibgen

g++ -O3 -pipe -Isrc/libs/liblinefetch -Isrc/libs/libtplreader \
    -Wno-deprecated-declarations \
    `pkg-config --cflags --libs libxml++-2.6` \
    src/artrepgen/*.cc src/libs/liblinefetch/*.cc src/libs/libtplreader/*.cc \
    -o src/artrepgen/artrepgen
