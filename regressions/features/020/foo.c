#include <stdlib.h>

void foo(void) {
    free(malloc(0x10));
}
