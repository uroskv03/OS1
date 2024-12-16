
#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/print.hpp"
#include "../h/syscall_c.hpp"


int TCB::broj = 0;

TCB *TCB::running = nullptr;

uint64 TCB::timeSliceCounter = 0;

TCB *TCB::createThread(Body body, void* arg, char* stack)
{
    return new TCB(body, TIME_SLICE, arg, stack);
}

void TCB::yield()
{
    __asm__ volatile("li a0, 0x50");
    __asm__ volatile ("ecall");
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished() && !old->isBlocked()) { Scheduler::put(old); }
    running = Scheduler::get();
    TCB::contextSwitch(&old->context, &running->context);

}

void TCB::threadWrapper(void* arg)
{
    Riscv::popSppSpie();
    running->body(running->arg);
    thread_exit();
}
