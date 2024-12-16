
#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/print.hpp"

//#include "../lib/mem.h"

enum InstructionCode : uint64{
    MEM_ALLOC = 0x01,
    MEM_FREE = 0x02,
    THREAD_CREATE = 0X11,
    THREAD_EXIT = 0X12,
    THREAD_DISPATCH = 0X13,
    SEM_OPEN = 0X21,
    SEM_CLOSE = 0X22,
    SEM_WAIT = 0X23,
    SEM_SIGNAL = 0X24,
    SEM_TIMEDWAIT = 0X25,
    SEM_TRYWAIT = 0X26,
    TIME_SLEEP = 0X31 ,
    GETC = 0X41 ,
    PUTC = 0X42,
    YIELD = 0x50,
    //POM = 0x51
};

void Riscv::popSppSpie()
{
    mc_sstatus(Riscv::SSTATUS_SPP);
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap()
{
    uint64 scause = r_scause();
    //uint64 volatile sepc = r_sepc();
    //uint64  volatile sstatus = r_sstatus();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        //sepc += 4;
        uint64 volatile sepc = r_sepc() + 4;
        uint64  volatile sstatus = r_sstatus();
        TCB::timeSliceCounter = 0;
        uint64 volatile code;
        __asm__ volatile("mv %0, a0" : "=r" (code));
        switch (code) {
            case(MEM_ALLOC):
                size_t size;
                __asm__ volatile("mv %0, a1" : "=r" (size));
                void* ret1;
                ret1 = MemoryAllocator::mem_alloc(size);
                __asm__ volatile("mv a0, %0" : : "r" (ret1));
                __asm__ volatile("sd a0, 80(fp)" : );
                break;

            case(MEM_FREE):
                void* p;
                __asm__ volatile("mv %0, a1" : "=r" (p));
                int ret2;
                ret2 = MemoryAllocator::mem_free(p);
                __asm__ volatile("mv a0, %0" : : "r" (ret2));
                __asm__ volatile("sd a0, 80(fp)" : );
                break;

            case(THREAD_CREATE):
                TCB** handle;
                void (*start_routine)(void *);
                void *arg;
                char* stack;
                __asm__ volatile("mv %0, a1" : "=r" (handle));
                __asm__ volatile("mv %0, a2" : "=r" (start_routine));
                __asm__ volatile("mv %0, a6" : "=r" (arg));
                __asm__ volatile("mv %0, a7" : "=r" (stack));
                int ret3 ;
                ret3 = 0;
                *handle = TCB::createThread(start_routine,arg, stack);
                if(*handle == nullptr) ret3 = -1;
                __asm__ volatile("mv a0, %0" : : "r" (ret3));
                __asm__ volatile("sd a0, 80(fp)" : );
                break;

            case(THREAD_EXIT):
                TCB::running->setFinished(true);
                TCB::dispatch();
                break;

            case(THREAD_DISPATCH):
                TCB::dispatch();
                break;

            case(SEM_OPEN):
                sem_t* handle1;   // da li sa pokazivacem?s
                unsigned int init;
                __asm__ volatile("mv %0, a1" : "=r" (handle1));
                __asm__ volatile("mv %0, a2" : "=r" (init));
                *handle1 = new Sem(init);
                int ret4;
                ret4 = 0;
                if(*handle1 == nullptr) ret4 = -1;
                __asm__ volatile("mv a0, %0" : : "r" (ret4));
                __asm__ volatile("sd a0, 80(fp)" : );
                break;

            case(SEM_CLOSE):
                sem_t handle2;
                __asm__ volatile("mv %0, a1" : "=r" (handle2));
                int ret5;
                ret5 = handle2->sem_close();
                __asm__ volatile("mv a0, %0" : : "r" (ret5));
                __asm__ volatile("sd a0, 80(fp)" : );
                break;

            case(SEM_WAIT):
                sem_t id1;
                __asm__ volatile("mv %0, a1" : "=r" (id1));
                int ret6;
                ret6 = id1->wait();
                __asm__ volatile("mv a0, %0" : : "r" (ret6));
                __asm__ volatile("sd a0, 80(fp)" : );
                break;

            case(SEM_SIGNAL):
                sem_t id2;
                __asm__ volatile("mv %0, a1" : "=r" (id2));
                int ret7;
                ret7 = id2->sem_signal();
                __asm__ volatile("mv a0, %0" : : "r" (ret7));
                __asm__ volatile("sd a0, 80(fp)" : );
                break;

            case(SEM_TIMEDWAIT):

                break;

            case(SEM_TRYWAIT):
                sem_t id3;
                __asm__ volatile("mv %0, a1" : "=r" (id3));
                int ret9;
                ret9 =  id3->tryWait();
                __asm__ volatile("mv a0, %0" : : "r" (ret9));
                __asm__ volatile("sd a0, 80(fp)" : );
                break;

            case(TIME_SLEEP):

                break;


            case(GETC):
                char ret14;
                ret14 = __getc();
                __asm__ volatile("mv a0, %0" : : "r" (ret14));
                __asm__ volatile("sd a0, 80(fp)" : );
                break;

            case(PUTC):
                char c;
                __asm__ volatile("mv %0, a1" : "=r" (c));
                __putc(c);
                break;

            case(YIELD):
                TCB::timeSliceCounter = 0;
               // uint64 volatile sstatus = r_sstatus();
                TCB::dispatch();
                //w_sstatus(sstatus);
                //w_sepc(sepc);
                break;
            /*case(POM):
                MemoryAllocator::prfree();
                break;*/
        }
        w_sstatus(sstatus);
        w_sepc(sepc);
    }


    else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
        TCB::timeSliceCounter++;
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();  //ASINHORNO
            w_sstatus(sstatus);
            w_sepc(sepc);
        }

    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();
        mc_sip(SIP_SSIP);
        console_handler();
        w_sstatus(sstatus);
        w_sepc(sepc);

    }
    else
    {
        uint64 volatile sepc = r_sepc();
        printString("\n");
        printString("scause: ");

        printInteger1((uint64) scause);
        printString("\n");
        printString("sepc: ");
        printInteger1((uint64) sepc);
        printString("\n");

        volatile uint32 *addr = (volatile uint32 *)0x100000;
        *addr = 0x5555;
    }

}