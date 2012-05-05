#include <stdlib.h>

/* Регрессионный тест на внутреннюю ошибку, захват-освобождение,
   захват-освобождение обрабатывался ошибочно */

int main(void) {
    char* p;

    art_start("");

    p = malloc(0x33);

    free(p);

    p = malloc(0x35);

    free(p);

    return 0;
}
