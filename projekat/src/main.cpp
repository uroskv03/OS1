//
// Created by marko on 20.4.22.
//

#include "../h/tcb.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"


#include "../lib/console.h"

extern void userMain();

int main()
{

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    TCB* runThread;// = new TCB(nullptr, 2,  nullptr);
    thread_create(&runThread, nullptr, nullptr);
    TCB::running = runThread;

    TCB* userMainThread;
    thread_create(&userMainThread, reinterpret_cast<void (*)(void *)>(userMain), nullptr);

    while(!userMainThread->isFinished()) TCB::yield();


    volatile uint32 *addr = (volatile uint32 *)0x100000;
    *addr = 0x5555;
    return 0;

}