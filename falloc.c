#include "falloc.h"
#include "error.h"
#include "log.h"

#include <string.h>
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
        #ifdef DEBUG
        LOG_ERROR("Memory allocation failed!");
        #endif  //DEBUG

        exit(EXIT_FAILURE);
    }

    void *ptr = aligned_alloc(block -> alignment, _total_aligned_size);

    memblk temp_blk = {
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


memblk internal_alloc(stack_allocate_t *alloc, 
                      uint64_t size,
                      const char *file,
                      int line)
{
    if(!IS_ALIGN(size, alloc -> alignment))
    {
        #ifdef DEBUG
        LOG_WARN("[%s:%d] Requested allocation size %llu bytes is not a multiple of the alignment size %u bytes. "
                 "Adjusting size for proper alignment.",
                 file, line, (unsigned long long)size, alloc->alignment);
        #endif  //DEBUG
    }

    uint64_t aligned_size = ALIGN(size, alloc -> alignment);

    memblk allocated_address_block;
    memory_address current_head = alloc -> top_pointer;
    memory_address next_head = add_memeory_address(current_head, aligned_size);

    if(__builtin_expect((next_head.index) > (alloc -> mem_end.index), false))
    {
        #ifdef DEBUG
        LOG_ERROR("Allocated size exceeds the block size limit.");
        #endif  //DEBUG

        exit(EXIT_FAILURE);
    }

    allocated_address_block.memptr = current_head.raw;
    allocated_address_block.size = aligned_size;

    alloc -> top_pointer = next_head;

    return allocated_address_block;
}


stack_allocate_t *_stack_create(uint64_t size,
                                alignment_t alignment,
                                const char *file,
                                int line)
{
    if(size < 512) {
        #ifdef DEBUG
        LOG_ERROR("[%s:%d] Error: Requested memory size %u bytes is too small. Minimum allowed size is 512 bytes.",
                  file, line, (unsigned int)size);
        #endif  //DEBUG

        exit(EXIT_FAILURE);
    }

    align_block_create_info_t align_block_info = {
        sizeof(stack_allocate_t),
        size,
        alignment
    };

    aligned_block_t block = create_aligned_block(&align_block_info);

    stack_allocate_t *alloc = (stack_allocate_t *)block.base_address.raw;

    alloc -> alloc_type  = stack;
    alloc -> alignment   = alignment;
    alloc -> top_pointer = block.usable_address;
    alloc -> mem_start   = block.usable_address;
    alloc -> mem_end     = add_memeory_address(block.usable_address, block.usable_size);
    alloc -> stack_size  = block.total_size;
    
    return alloc;
}


memblk _stack_alloc(stack_allocate_t *alloc, 
                       uint64_t size, 
                       const char *file, 
                       int line)
{
    if(alloc -> alloc_type == stack) {
        return internal_alloc(alloc, size, file, line);
    }
    
    memblk null_mem;
    null_mem.memptr = NULL;
    null_mem.size = 0;

    return null_mem;
}


void _stack_destroy(stack_allocate_t *destroy_block, 
                    const char *file, 
                    int line,
                    ...)
{
    va_list args;
    va_start(args, line);  // Initialize the argument list, starting after 'line'

    // Process each pointer passed in the variadic argument list
    while (true) {
        void **blockptr = va_arg(args, void**); // Get the next argument (a void pointer)

        // If we get a NULL pointer, stop the loop
        if (blockptr == NULL) {
            break;
        }

        // Free the memory and set the pointer to NULL
        if (destroy_block->alloc_type == stack) {
            free(destroy_block);
            destroy_block = NULL;
        }

        *blockptr = NULL;
    }

    va_end(args);  
}

void _stack_dealloc(stack_allocate_t *alloc, 
                       memblk block, 
                       const char *file,
                       int line)
{
    memory_address checkbuffer = subs_memeory_address(alloc -> top_pointer, block.size);

    if(__builtin_expect((checkbuffer.index) < (alloc -> mem_start.index), false))
    {
        fflush(stdout);
        #ifdef DEBUG
        LOG_ERROR("Deallocated size recedes the block size limit.");
        #endif  //DEBUG

        exit(EXIT_FAILURE);
    }


    if(checkbuffer.raw == block.memptr) {
        alloc -> top_pointer = checkbuffer;
    }
    
}

void _stack_dealloc_all(stack_allocate_t *alloc,
                           const char *file,
                           int line)
{
    alloc -> top_pointer = alloc ->mem_start;
}
