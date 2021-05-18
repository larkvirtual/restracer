#include <stdlib.h>

/* Перезахват памяти такого же объёма */

int main(void) {
    char* p;

    art_start("");

    p = realloc(NULL, 0x111);
    p = realloc(p, 0x111);

    free(p);

    return 0;
}
