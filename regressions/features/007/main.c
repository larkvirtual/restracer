/* Использвание ресурса после освобождения */

int main(void) {
    size_t value;
    FILE *f;

    art_start("");

    f = fopen("/dev/null", "rb");

    fclose(f);

    value = fwrite(0, 0, 0, f);

    return 0;
    }
