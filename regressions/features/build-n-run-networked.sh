#!/bin/sh

SED=$(type gsed > /dev/null && echo gsed || echo sed)

if ! [ -z "$CI_JOB_ID" ]; then
    # We are inside GitLab CI, setting other variables
    repGenStartupTime=16
    repGenShutdownTime=16
else
    # Generic run on developer machine
    repGenStartupTime=4
    repGenShutdownTime=4
fi
echo "repGenStartupTime='$repGenStartupTime'"
echo "repGenShutdownTime='$repGenShutdownTime'"

killall -9 artrepgen || true

passOK=0
passFAILED=0
FAILEDlist=""
total=0

for d in `find . -type d -name "0[0-9]*" | sort`; do
#    pushd . > /dev/null
    d=`echo $d | ${SED} 's/\.\///g' -`
    if [ "$d" = 019 ]; then
        RT_TEMPLATE=posix-gcc-mt-netw-special
    else
        RT_TEMPLATE=posix-gcc-mt-netw-lint
    fi

    artrepgen --sock 12345 >     "$d/.real" &
    ./usleep $repGenStartupTime

    cd "$d" && CFLAGS="${CFLAGS} -g -ggdb" RT_TEMPLATE=$RT_TEMPLATE rt-gmake \
    && ./main || true

    ../usleep $repGenShutdownTime
    killall -9 artrepgen || true
    ${SED} -r -i 's/[0-9A-Z]{16}//g' .real

    diff -u .real .right
    if [ $? -eq 0 ]; then
        rm .real
        passOK=$((passOK+1))
        echo "$i OK";
    elif [ -f .right-freebsd ]; then # FreeBSD specific behavior handling
        diff -u .real .right-freebsd
        if [ $? -eq 0 ]; then
            rm .real
            passOK=$((passOK+1))
            echo "$i OK";
        else
            passFAILED=$((passFAILED+1))
            echo "$i FAILED"
            FAILEDlist="$FAILEDlist $d"
        fi
    else
        passFAILED=$((passFAILED+1))
        echo "$i FAILED"
        FAILEDlist="$FAILEDlist $d"
    fi

    total=$((total+1))
 #   popd    > /dev/null
    cd ..
done

for d in `find . -type d -name "1[0-9]*" | sort`; do
#    pushd . > /dev/null
    d=`echo $d | ${SED} 's/\.\///g' -`
#    if [ "$d" = 019 ]; then
#        RT_TEMPLATE=posix-gcc-mt-netw-special
#    else
        RT_TEMPLATE=posix-gcc-mt-netw-lint
#    fi

    ./usleep $repGenStartupTime

    cd "$d" && CFLAGS="${CFLAGS} -g -ggdb" RT_TEMPLATE=$RT_TEMPLATE rt-gmake \
    && ./main                        .real || true

    ../usleep $repGenShutdownTime
    killall -9 artrepgen || true
    ${SED} -r -i 's/[0-9A-Z]{16}//g' .real

    diff -u .real .right
    if [ $? -eq 0 ]; then
        rm .real
        passOK=$((passOK+1))
        echo "$i OK";
    elif [ -f .right-freebsd ]; then # FreeBSD specific behavior handling
        diff -u .real .right-freebsd
        if [ $? -eq 0 ]; then
            rm .real
            passOK=$((passOK+1))
            echo "$i OK";
        else
            passFAILED=$((passFAILED+1))
            echo "$i FAILED"
            FAILEDlist="$FAILEDlist $d"
        fi
    else
        passFAILED=$((passFAILED+1))
        echo "$i FAILED"
        FAILEDlist="$FAILEDlist $d"
    fi

    total=$((total+1))
 #   popd    > /dev/null
    cd ..
done

echo '******************************************************************************'
echo '* WARNING: if template art_start_selfinit="false" => 018/main.c WILL FAIL!!! *'
echo "* THIS IS TOTALLY FINE, CUZ 018.c doesn't contain art_start()                *"
echo '******************************************************************************'

echo "-------------------------------------"
echo "TOTAL PASSED: $passOK/$total"
echo "TOTAL FAILED: $passFAILED"
if [ "$FAILEDlist" != "" ]; then
    echo "FAILED list: $FAILEDlist"
    exit 1
fi
