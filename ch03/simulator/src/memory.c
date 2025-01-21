#include "sim.h"

Memory memory = {0};

#define CACHE_SET 0

uint64_t read64bits(Memory *memory, uint64_t paddr)
{
    assert(paddr >= (uint64_t) memory->ram &&
           paddr < (uint64_t) (memory->ram + MEM_CAPACITY));

#if CACHE_SET
    return 0;
#endif

    return *(uint64_t *) paddr;
}

void write64bits(Memory *memory, uint64_t paddr, uint64_t data)
{
    assert(paddr >= (uint64_t) memory->ram &&
           paddr < (uint64_t) (memory->ram + MEM_CAPACITY));

#if CACHE_SET
    return;
#endif

    *(uint64_t *) paddr = data;
}

void reset_memory(Memory *memory)
{
    assert(memory != NULL);
    memset(memory, 0, sizeof(Memory));
}
