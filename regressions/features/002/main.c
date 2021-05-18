#include <stdlib.h>

/* Освобождение ранее освобождённого ресурса */

int main(void) {
    char* p;

    art_start("");

    p = malloc(33);

    free(p);

    /* ... */

    free(p);

    return 0;
}
