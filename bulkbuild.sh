#!/bin/sh

g++ -O3 -pipe -Ilibs/liblinefetch -Ilibs/libtplreader \
    `pkg-config --cflags --libs libxml++-2.6` \
    artlibgen/src/*.cc libs/liblinefetch/*.cc libs/libtplreader/*.cc \
    -o artlibgen/src/artlibgen

g++ -O3 -pipe -Ilibs/liblinefetch -Ilibs/libtplreader \
    `pkg-config --cflags --libs libxml++-2.6` \
    artrepgen/*.cc libs/liblinefetch/*.cc libs/libtplreader/*.cc \
    -o artrepgen/artrepgen
