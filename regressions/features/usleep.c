#include <unistd.h> /* usleep() */
#include <stdio.h>  /* printf() */
#include <stdlib.h> /* atoi() */

int main(int argc, char **argv) {
if(2 == argc) {
    useconds_t us;
    us = atoi(argv[1]) * 1000;
    return usleep(us);
    }
else {
    printf("Usage:   %s <ms>\n", 1 == argc ? argv[0] : "usleep");
    printf("Example: %s 5\n", 1 == argc ? argv[0] : "usleep");
    return 0;
    }
}
