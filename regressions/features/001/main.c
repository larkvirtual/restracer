#include <stdlib.h>

/* Освобождение незахваченного неправильного (NULL) ресурса */

int main(void) {
    art_start("");

    free((void*)0x00000000);

    return 0;
}
