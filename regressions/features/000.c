#include <stdlib.h>

/* Освобождение незахваченного правильного (не NULL) ресурса */

int main(void) {
    art_start("");

    free((void*)0x12345678);

    return 0;
}
