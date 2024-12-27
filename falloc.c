#include "falloc.h"
#include "error.h"
#include "log.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

static inline aligned_block_t create_aligned_block(align_block_create_info_t *block) 
{
    const uint64_t aligned_alloc_size = ALIGN(block -> allocator_size,
                                        block -> alignment);
    const uint64_t aligned_mem_size = ALIGN(block -> mem_size,
                                        block -> alignment);

    uint64_t total_aligned_size = 0;

    if(!(ADD_UINT64_SAFE(aligned_mem_size, 
                    aligned_alloc_size, &total_aligned_size)))
    {
        // Error handling
    }

    total_aligned_size = aligned_alloc_size + aligned_mem_size;

    size_t _total_aligned_size = (size_t)total_aligned_size;

    if(!aligned_alloc(block -> alignment, _total_aligned_size))
    {
        LOG_ERROR("Memory allocation failed!");
        exit(EXIT_FAILURE);
    }

    void *ptr = aligned_alloc(block -> alignment, _total_aligned_size);

    memblk temp_blk = {
        //aligned_alloc(block -> alignment, _total_aligned_size),
        ptr,
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


memblk internal_alloc(stack_allocate_t *alloc, uint64_t size)
{
    if(!IS_ALIGN(size, alloc -> alignment))
    {
        LOG_WARN("Requested allocation size %llu bytes is not a multiple of the alignment size %u bytes. Adjusting size for proper alignment.", (unsigned long long)size, alloc -> alignment);
    }

    uint64_t aligned_size = ALIGN(size, alloc -> alignment);

    memblk allocated_address_block;
    memory_address current_head = alloc -> top_pointer;
    memory_address next_head = add_memeory_address(current_head, aligned_size);

    if(__builtin_expect((next_head.index) > (alloc -> mem_end.index), false))
    {
        LOG_ERROR("Allocated size exceeds the block size limit.");
        exit(EXIT_FAILURE);
    }

    allocated_address_block.memptr = current_head.raw;
    allocated_address_block.size = aligned_size;

    alloc -> top_pointer = next_head;

    return allocated_address_block;
}


stack_allocate_t *stack_create(stack_alloc_info_t *info)
{
    //LOG_DEBUG("Error");
    if(info -> size < 512) {
        LOG_ERROR("Error: Requested memory size %u bytes is too small. Minimum allowed size is 512 bytes.",
                (unsigned int)info -> size);
        exit(EXIT_FAILURE);
    }

    align_block_create_info_t align_block_info = {
        sizeof(stack_allocate_t),
        info -> size,
        info -> alignment
    };

    aligned_block_t block = create_aligned_block(&align_block_info);

    stack_allocate_t *alloc = (stack_allocate_t *)block.base_address.raw;

    alloc -> alloc_type  = stack;
    alloc -> alignment   = info -> alignment;
    alloc -> top_pointer = block.usable_address;
    alloc -> mem_start   = block.usable_address;
    alloc -> mem_end     = add_memeory_address(block.usable_address, block.usable_size);
    alloc -> stack_size  = block.total_size;
    
    return alloc;
}


memblk stack_allocate(stack_allocate_t *alloc, uint64_t size)
{
    if(alloc -> alloc_type == stack) {
        return internal_alloc(alloc, size);
    }
    else {
        
    }
    
    memblk null_mem;
    null_mem.memptr = NULL;
    null_mem.size = 0;

    return null_mem;
}


void stack_destroy(stack_allocate_t *destroy_block)
{
    if(destroy_block -> alloc_type == stack) {
        free(destroy_block);    
        return ;
    }
    else {
        
    } 

    free(destroy_block);
    return ;
}
