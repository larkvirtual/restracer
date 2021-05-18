/* Использование незахваченного (умолчательное значение нерабочее) ресурса */
/* Вариант для умолчательного рабочего (good handle) значения в тесте 006.c */

int main(void) {
    size_t value;
    FILE *f;

    art_start("");

    f = NULL;

    value = fwrite(0, 0, 0, f);

    return 0;
}
