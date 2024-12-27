#include "falloc.h"
#include <stdio.h>

int main() {
    stack_alloc_info_t s;
    alignment_t a = b16;

    s.size = 512;
    s.alignment = a;

    stack_allocate_t *st = stack_create(&s);

    memblk m = stack_allocate(st, 24);

    int *ptr = (int *)m.memptr; 
    int size = m.size;

    //printf("%d\n", size);

    for(int i = 0; i < size - 1; i++) {
        ptr[i] = i;
    }

    for(int i = 0; i < size - 1; i++) {
        printf("%d\n", ptr[i]);
    }

    return 0;
}
