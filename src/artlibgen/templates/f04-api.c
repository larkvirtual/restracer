#include "f04-api.h"

int brokenAllocator(char* z) {
    (void)z;

    return 0x33;
}

void someDeallocator(int z) {
    (void)z;
}
