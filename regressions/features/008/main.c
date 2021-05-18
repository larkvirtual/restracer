#include <stdlib.h>

/* Неиспользование ресурса с момента захвата до момента освобождения */

int main(void) {
    char* p;

    art_start("");

    p = malloc(0x33);

    free(p);

    return 0;
}
