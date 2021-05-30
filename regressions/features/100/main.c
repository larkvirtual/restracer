#include <stdlib.h>

/* Освобождение незахваченного правильного (не NULL) ресурса */

int main(int argc, char **argv) {
    char mybuff[256] = "";

    if(argc != 2) {
        printf("Usage:   %s filename\n", argv[0]);
        printf("Example: ./main .real\n");
        return 0;
    }

    /* possible buffer overflow! */
    strcpy(mybuff, argv[0] ? argv[0] : "argv[0]-less application");
    strcat(mybuff, ";");

    strcat(mybuff, "autostart;");

    strcat(mybuff, argv[1]);

    art_start(mybuff);

    free((void*)0x12345678);

    return 0;
}
