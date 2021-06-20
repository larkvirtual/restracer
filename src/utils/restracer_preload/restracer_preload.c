#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>

static void* (*real_malloc)(size_t size);
static void  (*real_free)(void *ptr);
static void* (*real_art_malloc)(size_t size, char*, size_t);

/* DO NOT USE printf(), fprintf(stdout) IT WILL SEGFAULT!!! */

__attribute__((constructor))
static void init() {
    fprintf(stderr, "0\n");
        real_malloc = dlsym(RTLD_NEXT, "malloc");
    fprintf(stderr, "1\n");
        real_free   = dlsym(RTLD_NEXT, "free");
    fprintf(stderr, "2\n");
        real_art_malloc = dlsym(RTLD_NEXT, "art_malloc");
    fprintf(stderr, "3\n");

    fprintf(stderr, "real_malloc = %p\n", real_malloc);
    fprintf(stderr, "real_art_malloc = %p\n", real_art_malloc);
    fprintf(stderr, "init\n");
}

int is_restracer_call(void *real_art, void *actual_call) {
    ssize_t r_art, a_call, delta;
    int     retaddr_reasonable_near_foo_start;

    r_art = (ssize_t)real_art;
    a_call = (ssize_t)actual_call;

    delta = a_call - r_art;
    if(delta < 0) delta *= -1;
    if(delta < 100) retaddr_reasonable_near_foo_start = 1;
    else            retaddr_reasonable_near_foo_start = 0;

    return retaddr_reasonable_near_foo_start;
}

void *malloc(size_t size) {
    void *bra = __builtin_return_address(0);
    if(!is_restracer_call(real_art_malloc, bra)) {
        char *c = 0;
        fprintf(stderr, "calling not from restracer wrapper (%p), but from %p\n", real_art_malloc, bra);
        fprintf(stderr, "delta from restracer wrapper is %ld bytes\n", (ssize_t)real_art_malloc - (ssize_t)bra);
        fprintf(stderr, "trapping to backtrace...\n");
        *c = 0x33;
    }

    void *ptr = real_malloc(size);
    fprintf(stderr, "malloc(%zd) = %p\n", size, ptr);
    return ptr;
}

void free(void *ptr) {
    real_free(ptr);
    fprintf(stderr, "free(%p)\n", ptr);
}
