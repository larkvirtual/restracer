#!/bin/sh -e

if [ $# -eq 0 ]; then echo "Usage: $0 tag"; exit 0; fi

case `uname -s` in
    Linux)
        OS=linux;;
    FreeBSD)
        OS=freebsd;;
    SunOS)
        OS=nexenta;;
    *)
        OS=unknown;;
esac

art_dir=art-$1-$OS-`uname -m`
art_dir=`echo $art_dir | sed 's/i[3-6]86/x86/g'`
art_dir=`echo $art_dir | sed 's/mips64/mipsel/g'`
cd src && scons -Q -j4 OS=$OS STATIC=0 RELEASE=1 && cd .. && mkdir $art_dir

if [ ! $? -eq 0 ]; then echo "Build failure." exit 1; fi

cp src/artlibgen/src/artlibgen $art_dir &&
cp src/artrepgen/artrepgen $art_dir &&
strip $art_dir/* &&
cp src/artlibgen/templates/posix-gcc-mt-file-lint.xml $art_dir &&
cp src/make-distro-Makefile $art_dir/Makefile &&
cp regressions/features/003.c $art_dir/000.c &&
tar cf $art_dir.tar $art_dir &&
cp $art_dir.tar $art_dir.tar- &&
gzip -9 $art_dir.tar &&
#cp $art_dir.tar- $art_dir.tar &&
#bzip2 -9 $art_dir.tar &&
mv $art_dir.tar- $art_dir.tar &&
7z a -mx=9 $art_dir.7z $art_dir &&
rm $art_dir.tar && rm -rf $art_dir

if [ $OS = "linux" ]; then
    md5sum $art_dir.tar.* $art_dir.7z > $art_dir.CHECKSUM.md5; fi
if [ $OS = "nexenta" ]; then
    md5sum $art_dir.tar.* $art_dir.7z > $art_dir.CHECKSUM.md5; fi
if [ $OS = "freebsd" ]; then
    md5 -r $art_dir.tar.* $art_dir.7z > $art_dir.CHECKSUM.md5;
    sed 's/ /  /g' $art_dir.CHECKSUM.md5 > tmp;
    mv tmp $art_dir.CHECKSUM.md5; # make it suitable for gnu md5sum -c
fi
