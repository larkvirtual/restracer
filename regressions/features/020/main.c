#include <stdlib.h>
#include "foo.h"

/* Detect non-instrumented resource operationg function call (broken Makefile,
   something missing, etc...) */

void bar(void) {
    puts("Real bar");
}

int main(void) {
    puts("pre");
    art_start("");
    puts("post");

    foo();
    puts("prefinal");

    bar();
    puts("final");

    return 0;
}
