for i in 0 1 2 3; do
    cc -O$i -o bra-O$i bra.c
    ./bra-O$i > bra-O$i.txt
done

grep "Is retaddr_reasonable_near_foo_start: YES" *.txt

rm -f *.txt bra-O{0,1,2,3}
