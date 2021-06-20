#include <stdio.h>

void* baz(void) {
    printf("%p\n",__builtin_return_address(0));
    return __builtin_return_address(0);
    printf("%p\n",__builtin_frame_address(0));
}

void* foo(void) {
    return baz();
}

void* foo_terminator(void) {
    return foo_terminator;
}

void* bar(void) {
    return baz();
}

void* bar_terminator(void) {
    return bar_terminator;
}

int main(){
    void* call_addr = foo();
    printf("main() = %p\n", main);
    printf("foo() = %p\n", foo);
    printf("foo_terminator() = %p\n", foo_terminator);
    printf("bar() = %p\n", bar);
    printf("bar_terminator() = %p\n", bar_terminator);

    puts("");

    int terminator_is_more;
    if((size_t)foo_terminator > (size_t)foo) terminator_is_more = 1;
    else                                     terminator_is_more = 0;
    printf("Is foo_terminator > foo:              %s\n", terminator_is_more ? "YES" : "NO");

        int retaddr_reasonable_near_foo_start;
        ssize_t delta = (ssize_t)call_addr - (ssize_t)foo;
        if(delta < 0) delta *= -1;
        if(delta < 100) retaddr_reasonable_near_foo_start = 1;
        else            retaddr_reasonable_near_foo_start = 0;
        printf("Is retaddr_reasonable_near_foo_start: %s\n", retaddr_reasonable_near_foo_start ? "YES" : "NO");
        printf("delta = %d\n", delta);

    if(terminator_is_more){
        ssize_t foo_len = (ssize_t)foo_terminator - (ssize_t)foo;
        printf("foo len = %d\n", foo_len);
        printf("Is delta is less, then foo len:       %s\n", delta < foo_len ? "YES" : "NO");
    }
}
