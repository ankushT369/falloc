/* This code implements the stack allocation */
#ifndef __FALLOC_H__
#define __FALLOC_H__

#include <stdint.h>
#include <stdarg.h>

// ALIGN macro aligns the size to its specific requirements
#define ALIGN(_size, _alignment) ({ (_size + _alignment - 1) & ~(_alignment - 1); })

#define IS_ALIGN(_size, _alignment) (((_size) & ((_alignment) - 1)) == 0)

#define ADD_UINT64_SAFE(_arg1, _arg2, _result) ({                      \
    ((_arg1) > UINT64_MAX - (_arg2)) ? 0 : ((*_result) = (_arg1) + (_arg2), 1); \
})

#ifdef DEBUG
#define stack_create(size, alignment) _stack_create(size, alignment, __FILE__, __LINE__)

#define stack_alloc(stack, size) _stack_alloc(stack, size, __FILE__, __LINE__)

#define stack_destroy(stack, ...) _stack_destroy(stack, __FILE__, __LINE__, __VA_ARGS__)

#define stack_dealloc(stack, mem) _stack_dealloc(stack, mem, __FILE__, __LINE__);

#define stack_deallo_all(stack) _stack_dealloc_all(stack, __FILE__, __LINE__);

#else

#define stack_create(size, alignment) _stack_create(size, alignment, NULL, 0)

#define stack_alloc(stack, size) _stack_alloc(stack, size, NULL, 0)

#define stack_destroy(stack, ...) _stack_destroy(stack, NULL, 0, __VA_ARGS__)

#define stack_dealloc(stack, memblk) _stack_dealloc(stack, memblk, NULL, 0);

#define stack_dealloc_all(stack) _stack_dealloc_all(stack, NULL , 0);

#endif  // DEBUG


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
} alignment_t;

//typedef struct stack_allocate_t stack_allocate_t;
typedef enum alloc_type_e {none = 0, linear, stack, pool,} alloc_type_e;

typedef struct memory_address
{
    union
    {
        void *raw;
        uintptr_t index;
    };
} memory_address;


static inline memory_address add_memeory_address(memory_address address, 
                                                 uint64_t size)
{
    address.index = address.index + size;
    return address;
}

static inline memory_address subs_memeory_address(memory_address address, 
                                                 uint64_t size)
{
    address.index = address.index - size;
    return address;
}

typedef struct stack_allocate_t
{
    alloc_type_e alloc_type;
    alignment_t alignment;
    memory_address top_pointer;
    memory_address mem_start;
    memory_address mem_end;
    uint64_t stack_size;
    //stack_allocate_t *parent;
} stack_allocate_t;


typedef struct aligned_block_t
{
    memory_address base_address;
    uint64_t total_size;
    memory_address usable_address;
    uint64_t usable_size;
} aligned_block_t;


typedef struct align_block_create_info_t
{
    //stack_allocate_t *parent; // ???
    uint64_t allocator_size;
    uint64_t mem_size;
    alignment_t alignment;
} align_block_create_info_t;


/* 
 * This is a memblk struct returned 
 * by the allocate function to store values
 *
 */
typedef struct memblk
{
    union
    {
        void* memptr;
        uintptr_t memaddr;
    };
    uint64_t size; 
} memblk;


/* stack API for falloc */
stack_allocate_t *_stack_create(uint64_t, alignment_t, const char *file, int line);
memblk _stack_alloc(stack_allocate_t *, uint64_t, const char *file, int line);
void _stack_destroy(stack_allocate_t *, const char *file, int line, ...);
void _stack_dealloc(stack_allocate_t *, memblk, const char *file, int line);
void _stack_dealloc_all(stack_allocate_t *, const char *file, int line);

/* pool API for falloc */


#endif //__FALLOC_H__
