#include "falloc.h"
#include "error.h"

#include <stddef.h>
#include <stdlib.h>

typedef enum alloc_type_e : uint64_t {none = 0, linear, stack, pool,} alloc_type_e;

typedef struct memory_address {
    union {
        void *raw;
        uintptr_t index;
    };
} memory_address;

static inline memory_address add_memeory_address(memory_address address, uint64_t size) {
    address.index = address.index + size;
    return address;
}


struct stack_allocate_t {
    alloc_type_e alloc_type;
    alignment_t alignment;
    memory_address top_pointer;
    memory_address mem_start;
    memory_address mem_end;
    uint64_t stack_size;
    //s_allocate_t *parent;
};


typedef struct aligned_block_t {
    memory_address base_address;
    uint64_t total_size;
    memory_address usable_address;
    uint64_t usable_size;
} aligned_block_t;


typedef struct align_block_create_info_t {
    //s_allocate_t *parent; // ???
    uint64_t allocator_size;
    uint64_t mem_size;
    alignment_t alignment;
} align_block_create_info_t;


static inline aligned_block_t create_aligned_block(align_block_create_info_t *block) {
    const uint64_t aligned_alloc_size = ALIGN(block -> allocator_size,
                                        block -> alignment);
    const uint64_t aligned_mem_size = ALIGN(block -> mem_size,
                                        block -> alignment);

    uint64_t total_aligned_size = 0;

    if(!(ADD_UINT64_SAFE(aligned_mem_size, 
                    aligned_alloc_size, &total_aligned_size))) {
        // Error handling
    }

    total_aligned_size = aligned_alloc_size + aligned_mem_size;

    size_t _total_aligned_size = (size_t)total_aligned_size;

    memblk temp_blk = {
        aligned_alloc(block -> alignment, _total_aligned_size),
        total_aligned_size,
    };


    memory_address base_address, usable_address;

    base_address.raw = temp_blk.memptr;
    usable_address = add_memeory_address(base_address, aligned_alloc_size);

    uint64_t usable_size = aligned_mem_size;

    aligned_block_t aligned_block;

    aligned_block.base_address   = base_address;
    aligned_block.total_size     = temp_blk.size;
    aligned_block.usable_address = usable_address;
    aligned_block.usable_size    = usable_size;

    return aligned_block;
}


s_allocate_t *s_create(stack_alloc_info_t *info) {
    align_block_create_info_t align_block_info = {
        sizeof(s_allocate_t),
        info -> size,
        info -> alignment
    };

    aligned_block_t block = create_aligned_block(&align_block_info);

    s_allocate_t *alloc = (s_allocate_t *)block.base_address.raw;

    alloc -> alloc_type  = stack;
    alloc -> alignment   = info -> alignment;
    alloc -> top_pointer = block.base_address;
    alloc -> mem_start   = block.usable_address;
    alloc -> mem_end     = add_memeory_address(block.usable_address, block.usable_size);
    alloc -> stack_size  = block.total_size;
    
    return alloc;
}
