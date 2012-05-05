#include <stdlib.h>

/* Захват памяти через realloc и её дальнейшая утечка */

int main(void) {
    char* p;

    art_start("");

    p = realloc(NULL, 0x44000);

    return 0;
}
