#include <stdlib.h>

/* wild handler deallocation */

int main(void) {
    free((void*)0x12345678);

    return 0;
}
