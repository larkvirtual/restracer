#!/bin/sh -ex

ARTROOT=../..
ARTLIBGEN=$ARTROOT/src/artlibgen/src/artlibgen
ARTREPGEN=$ARTROOT/src/artrepgen/artrepgen
#CCFLAGS="-W -Wall -Wextra -ansi"
CCFLAGS=""

rm -f art.* [0-9][0-9][0-9].c.out

if [ ! -f $ARTLIBGEN ]; then
    echo "$ARTLIBGEN is missing!";
    exit -1
fi

if [ ! -f $ARTREPGEN ]; then
    echo "$ARTREPGEN is missing!";
    exit -1
fi

$ARTLIBGEN $ARTROOT/src/artlibgen/templates/posix-gcc-mt-file-lint.xml art.h art.c &&
cc -c art.c -o art.o -g -ggdb -Wno-pointer-to-int-cast

passOK=0
passFAILED=0
FAILEDlist=""
total=0

for i in `ls [0-9][0-9][0-9].c`; do
#    set -x
    cc ${CCFLAGS} -g -ggdb -include art.h $i art.o -o $i.out
    ./$i.out
    $ARTREPGEN --file tracefile.out > tmp
#    exit
    sed -r 's/[0-9A-Z]{16}//g' tmp > $i.artrep.real
    rm -f tmp
    diff -u $i.artrep.real $i.artrep.right
    if test $? -eq 0; then
        rm $i.artrep.real;
        cp tracefile.out tracefile.out-$i
        passOK=$((passOK+1))
        echo "$i OK";
        cp art.c art.c-$i
        cp art.h art.h-$i
    elif [ -f $i.artrep.right-freebsd ]; then # FreeBSD specific behavior handling
        diff -u $i.artrep.real $i.artrep.right-freebsd
        if test $? -eq 0; then
            rm $i.artrep.real;
            cp tracefile.out tracefile.out-$i
            passOK=$((passOK+1))
            echo "$i OK";
            cp art.c art.c-$i
            cp art.h art.h-$i
        else
            passFAILED=$((passFAILED+1))
            echo "$i FAILED"
            FAILEDlist="$FAILEDlist $i"
            cp tracefile.out tracefile.out-$i
            cp art.c art.c-$i
            cp art.h art.h-$i
        fi
    else
        passFAILED=$((passFAILED+1))
        echo "$i FAILED"
        FAILEDlist="$FAILEDlist $i"
        cp tracefile.out tracefile.out-$i
        cp art.c art.c-$i
        cp art.h art.h-$i
    fi

    total=$((total+1))
done

##############################################################################
# А теперь для особых случаев

rm -f art.[cho]
$ARTLIBGEN $ARTROOT/src/artlibgen/templates/posix-gcc-mt-file-special.xml art.h art.c &&
cc -c art.c -o art.o -g -Wno-pointer-to-int-cast &&

for i in `ls f[0-9][0-9].c`; do
    cc ${CCFLAGS} -g f04-api.c -c
    cc ${CCFLAGS} -g -include art.h $i art.o f04-api.o -o $i.out -I.
    ./$i.out
    $ARTREPGEN --file tracefile.out > tmp
    sed -r 's/[0-9A-Z]{16}//g' tmp > $i.artrep.real
    rm -f tmp
    diff -u $i.artrep.real $i.artrep.right
    if test $? -eq 0; then
        rm $i.artrep.real;
        passOK=$((passOK+1))
        echo "$i OK";
    else
        passFAILED=$((passFAILED+1))
        echo "$i FAILED";
        FAILEDlist="$FAILEDlist $i"
    fi

    total=$((total+1))
done

echo '*************************************************************************'
echo '* WARNING: if template art_start_selfinit="false" => 018.c WILL FAIL!!! *'
echo "* THIS IS TOTALLY FINE, CUZ 018.c doesn't contain art_start()           *"
echo '*************************************************************************'

echo "-------------------------------------"
echo "TOTAL PASSED: $passOK/$total"
echo "TOTAL FAILED: $passFAILED"
if [ "$FAILEDlist" != "" ]; then
    echo "FAILED list: $FAILEDlist"
    exit 1
fi
