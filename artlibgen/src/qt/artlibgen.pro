# Application itself
SOURCES += ../gen_c_body.cc ../gen.cc  ../gen_c.cc ../artlibgen.cc
HEADERS += ../gen_c_body.h  ../gen_c.h ../gen.h

# "Lib linefetch"
SOURCES += ../../../libs/liblinefetch/utils_linefetch.cc
HEADERS += ../../../libs/liblinefetch/utils_linefetch.h

# "Lib tplreader"
SOURCES += ../../../libs/libtplreader/art_dump_tool.cc \
../../../libs/libtplreader/art_struct.cc \
../../../libs/libtplreader/ccloader.cc \
../../../libs/libtplreader/gen_xml_sax_read.cc

HEADERS += ../../../libs/libtplreader/art_dump_tool.h \
../../../libs/libtplreader/art_struct.h \
../../../libs/libtplreader/ccloader.h \
../../../libs/libtplreader/gen_xml_sax_read.h \
../../../libs/libtplreader/art_defs.h

INCLUDEPATH = ../../../libs/libtplreader ../../../libs/liblinefetch

QT =

QMAKE_CXXFLAGS += -Wno-deprecated-declarations
# prevent libxml++-2.6 warnings
CONFIG += link_pkgconfig
PKGCONFIG += libxml++-2.6
