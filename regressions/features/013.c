#include <stdlib.h>

/* Странное использование realloc() (и malloc(), хоть его сдесь и нет),
   утечка памяти */

int main(void) {
    char* p;

    art_start("");

    p = realloc(NULL, 0);

    return 0;
}
