
#include "../h/MemoryAllocator.hpp"
#include "../lib/console.h"
#include "../h/print.hpp"


MemoryAllocator::Block* MemoryAllocator::free = (MemoryAllocator::Block*)HEAP_START_ADDR;
bool MemoryAllocator::IsInicialized = false;
size_t MemoryAllocator::header_size = sizeof (Block);

void* MemoryAllocator::mem_alloc(size_t br) {
    size_t size = br * MEM_BLOCK_SIZE;

    if (!IsInicialized) {
        free = (MemoryAllocator::Block*)HEAP_START_ADDR;
        free->sled = nullptr;
        free->size = (size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR;
        IsInicialized = true;
    }

    Block* pom = free;
    Block* prev = nullptr;

    while (pom != nullptr) {
        if (pom->size < size) {
            prev = pom;
            pom = pom->sled;
        } else {
            if ((pom->size - size) < MEM_BLOCK_SIZE) {
                if (prev == nullptr) {
                    free = free->sled;
                } else {
                    prev->sled = pom->sled;
                }
                pom->sled = nullptr;
                return (void*)((char*)pom + header_size);
            } else {
                Block* nextBlock = (Block*)((char*)pom + size);
                nextBlock->sled = pom->sled;
                nextBlock->size = pom->size - size;
                pom->sled = nullptr;
                pom->size = size;
                if (prev == nullptr) {
                    free = nextBlock;
                } else {
                    prev->sled = nextBlock;
                }
                return (void*)((char*)pom + header_size);
            }
        }
    }
    return nullptr;
}


void* MemoryAllocator::mem_alloc1 (size_t size) {
    int br = (int)((size + header_size) / MEM_BLOCK_SIZE + ((size + header_size) % MEM_BLOCK_SIZE != 0));
    return mem_alloc(br);
}

int MemoryAllocator::mem_free(void *p) {
    if (p == nullptr) return -1;

    Block* q = (Block*)((char*)p - header_size);
    if (reinterpret_cast<char*>(q) < (char*)(HEAP_START_ADDR) ||
        reinterpret_cast<char*>(q) >= (char*)(HEAP_END_ADDR)) {
        return -2;
    }

    Block* pom = free;
    Block* prev = nullptr;
    while (pom != nullptr && pom < q) {
        prev = pom;
        pom = pom->sled;
    }

    if (prev == nullptr) {
        if (free == nullptr) {
            free = q;
        } else {
            q->sled = free;
            free = q;
        }
    } else {
        q->sled = pom;
        prev->sled = q;
    }

    tryToJoin(q);
    tryToJoin(prev);

    return 0;
}

bool MemoryAllocator::tryToJoin(MemoryAllocator::Block *r) {
    if (r == nullptr) return false;
    if (r->sled == nullptr) return false;
    Block* p = r->sled;
    if ((char*)r + r->size == (char*)p) {
        r->sled = p->sled;
        r->size += p->size;
        return true;
    }
    return false;
}

void MemoryAllocator::prfree() {
    Block* pom = free;
    while (pom != nullptr){
        printInteger1(pom->size);
        __putc('\n');
        pom = pom->sled;
    }
    __putc('k');
    __putc('\n');
}