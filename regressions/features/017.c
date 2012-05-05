#include <unistd.h>

int main(void) {
    art_start("");

    fork();

    return 0;
}
