#include "falloc.h"

enum alloc_type_e : uint64_t {none = 0, linear, stack, pool,};


struct stack_allocate_t {
         
};

typedef struct align_block_create_info {
    s_allocate_t *parent; // ???
    // The metadata size
    uint64_t allocator_size;
    // User-defined size allocation
    uint64_t mem_size;
    // User-defined alignment
    alignment_t aligned_value;
} align_block_create_info;


