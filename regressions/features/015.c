#include <stdlib.h>

/* Попытка перераспределения <<дикой>> памяти */

int main(void) {
    char* p;

    art_start("");

    p = realloc((void*)0xdeadbeef, 0x111);

    return 0;
}
