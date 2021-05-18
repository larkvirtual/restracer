#include <stdlib.h>

/* Утечка ресурса (для памяти с использованием float1, float2) */

int main(void) {
    char* p;

    art_start("");

    p = calloc(0x33, 1);

    return 0;
}
