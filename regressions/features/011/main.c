#include <stdlib.h>

/* Утечка ресурса при использовании realloc() */

int main(void) {
    char* p;
    char* z;

    art_start("");

    p = malloc(0x33);

    z = realloc(p, 0x44000);

    return 0;
}
