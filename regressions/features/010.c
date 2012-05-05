#include <stdlib.h>

/* Освобождение памяти через realloc() */

int main(void) {
    char* p;
    char* z;

    art_start("");

    p = malloc(0x33);

    z = realloc(p, 0);

    return 0;
}
