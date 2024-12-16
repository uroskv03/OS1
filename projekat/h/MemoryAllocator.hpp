#ifndef MEMORY_ALLOCATOR_HPP
#define MEMORY_ALLOCATOR_HPP
#include "../lib/hw.h"

class MemoryAllocator {
public:
    static size_t header_size;
    static void* mem_alloc (size_t size);
    static void* mem_alloc1 (size_t size);
    static int mem_free (void* p);
    static void prfree();

    MemoryAllocator(MemoryAllocator const&) = delete;
    void operator=(MemoryAllocator const&) = delete;
private:
    MemoryAllocator() = default;
    ~MemoryAllocator() = default;

    struct Block {
        Block* sled;
        size_t size;
    };

    static bool tryToJoin(Block* r);
    static bool IsInicialized;
    static Block* free;
};

#endif //MEMORY_ALLOCATOR_HPP