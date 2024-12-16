
#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

#include "../lib/hw.h"
#include "../h/scheduler.hpp"
//#include "../h/syscall_cpp.hpp"


#include "../h/MemoryAllocator.hpp"
#include "../lib/console.h"

// Thread Control Block
class TCB
{
public:
    static int broj;

    ~TCB() { delete[] stack; }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    bool isBlocked() const { return blocked; }

    void setBlocked(bool value) { blocked = value; }

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)(void*);

    static TCB *createThread(Body body,void* arg, char* stack);

    static void yield();

    static TCB *running;

    TCB(Body body,uint64 timeSlice, void* arg) :
            body(body),
            stack(body != nullptr ? new char[STACK_SIZE] : nullptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    }),
            timeSlice(timeSlice),
            finished(false),
            blocked(false),
            arg(arg)
    {
        if (body != nullptr) { Scheduler::put(this); }
    }

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



    TCB(Body body, uint64 timeSlice, void* arg, char* stack) :
            body(body),
            /*stack(body != nullptr ? new uint64[STACK_SIZE] : nullptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    }),*/
            stack(stack - DEFAULT_STACK_SIZE),
            context({(uint64) &threadWrapper,
                     (uint64) stack
                    }),
            timeSlice(timeSlice),
            finished(false),
            blocked(false),
            arg(arg)

    {
        broj++;
        if (body != nullptr) { Scheduler::put(this);
        }

    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    char *stack;
    Context context;
    uint64 timeSlice;
    bool finished;
    bool blocked;
    void* arg;

    friend class Riscv;

    static void threadWrapper(void* arg);

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static uint64 timeSliceCounter;

    static uint64 constexpr STACK_SIZE = 1024;
    static uint64 constexpr TIME_SLICE = 2;
};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
