#include <stdlib.h>

/* Утечка ресурса */

int main(void) {
    char* p;

    art_start("");

    p = malloc(33);

    return 0;
}
