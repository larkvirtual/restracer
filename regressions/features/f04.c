#include <stdlib.h>

/* Проверка бракованый API: захват такого же ресурса без его освобождения */

int main(void) {
    int p, z;

    art_start("");

    p = brokenAllocator("just");
    z = brokenAllocator("something");

    return 0;
}
