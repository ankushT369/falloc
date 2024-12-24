/* This code implements the stack allocation */
#ifndef FALLOC_H
#define FALLOC_H

#include <stdint.h>

// ALIGN macro aligns the size to its specific requirements
#define ALIGN(_size, _alignment) ({ (_size + _alignment - 1) & ~(_alignment - 1); })

typedef enum : uint64_t {
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
} alignment_t;

typedef struct s_allocate_t s_allocate_t;


/* 
 * This is a memblk struct returned 
 * by the allocate function to store values
 *
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
    alignment_t alignment;
} stack_alloc_info;

/* API for falloc */
s_allocate_t *s_create(stack_alloc_info *);
memblk s_allocate(s_allocate_t *);
void s_destroy(stack_alloc_info *);
void s_deallocate(memblk *);
void s_deallocate_all(s_allocate_t *);


#endif //FALLOC_H
