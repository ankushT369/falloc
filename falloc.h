/* This code implements the stack allocation */
#ifndef FALLOC_H
#define FALLOC_H

#include <stdint.h>


typedef enum {
    b4   = (1 << 2),
    b8   = (1 << 3),
    b16  = (1 << 4),
    b32  = (1 << 5),
    b64  = (1 << 6),
    b128 = (1 << 7),
    b256 = (1 << 8),
    b512 = (1 << 9),
    b1k  = (1 << 10),
    b4k  = (1 << 14),
} alignments;

struct s_allocate_t;

/* 
 * This is a memblk struct returned 
 * by the allocate function to store values
 */
typedef struct memblk {
    // pointer to the empty memory
    void* mem;
    // size of the empty memory
    uint64_t size; 
} memblk;

/**/
typedef struct stack_alloc_info {
    // p_allocator_t parent;
    // required size by user
    uint64_t size;
    // required alignment by user
    alignments _value;
} stack_alloc_info;

/* API for falloc */

/*
 * s_allocate_t *create takes stack_alloc_info* 
 * as an argument defined by user and returns
 * s_allocate_t*
 * */
s_allocate_t *create(stack_alloc_info *);
/* */
void destroy();
/* */
memblk allocate();
/* */
void deallocate();
/* */
void deallocate_all();


#endif //FALLOC_H
