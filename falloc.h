/* This code implements the stack allocation */
#ifndef FALLOC_H
#define FALLOC_H

#include <stdint.h>

// ALIGN macro aligns the size to its specific requirements
#define ALIGN(_size, _alignment) ({ (_size + _alignment - 1) & ~(_alignment - 1); })

#define IS_POWER_OF_2(n) (((n) != 0) && (((n) & ((n)-1)) == 0))

#define ADD_UINT64_SAFE(_arg1, _arg2, _result) ({                      \
    ((_arg1) > UINT64_MAX - (_arg2)) ? 0 : ((*_result) = (_arg1) + (_arg2), 1); \
})


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

typedef struct stack_allocate_t stack_allocate_t;


/* 
 * This is a memblk struct returned 
 * by the allocate function to store values
 *
 */
typedef struct memblk {
    // pointer to the empty memory
    union {
        void* memptr;
        uintptr_t memaddr;
    };
    // size of the empty memory
    uint64_t size; 
} memblk;

/**/
typedef struct stack_alloc_info_t {
    // p_allocator_t parent;
    // required size by user
    uint64_t size;
    // required alignment by user
    alignment_t alignment;
} stack_alloc_info_t;


/* API for falloc */
stack_allocate_t *s_create(stack_alloc_info_t *);
memblk stack_allocate(stack_allocate_t *, uint64_t);
void stack_destroy(stack_alloc_info_t *);
void stack_deallocate(memblk *);
void stack_deallocate_all(stack_allocate_t *);


#endif //FALLOC_H
