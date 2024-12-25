#include "falloc.h"

typedef enum alloc_type_e : uint64_t {none = 0, linear, stack, pool,} alloc_type_e;

typedef struct memory_address {} memory_address;

struct stack_allocate_t {
    alloc_type_e alloc_type;
    alignment_t alignment;
    memory_address top_pointer;
    memory_address mem_start;
    memory_address mem_end;
    uint64_t stack_size;
    s_allocate_t *parent;
};

typedef struct align_block_t {
     
} align_block_t;

typedef struct align_block_create_info {
    s_allocate_t *parent; // ???
    uint64_t allocator_size;
    uint64_t mem_size;
    alignment_t aligned_value;
} align_block_create_info;


