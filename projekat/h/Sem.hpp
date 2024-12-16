
#ifndef PROJECT_BASE_SEM_HPP
#define PROJECT_BASE_SEM_HPP

#include "../h/list.hpp"
#include "../h/tcb.hpp"

class Sem {
    typedef Sem* sem_t;
public:

    Sem (unsigned init = 1){ this->val = (int) init;}
    ~Sem ();
    int sem_close ();

    int wait ();
    int sem_signal ();

    int timedWait (time_t);
    int tryWait();

    using size_t = decltype(sizeof(0));

    void* operator new (size_t size) {
        return MemoryAllocator::mem_alloc1(size);
    }

    void operator delete (void* p) {
        MemoryAllocator::mem_free(p);
    }

    void* operator new[] (size_t size) {
        return MemoryAllocator::mem_alloc1(size);
    }

    void operator delete[] (void* p){
        MemoryAllocator::mem_free(p);
    }

private:
    void block();
    void deblock();
    int val = 1;
    int br = 0;
    volatile bool closed = false;
    List<TCB> blocked;

};

#endif //PROJECT_BASE_SEM_HPP
