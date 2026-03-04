# Kernel- Operating system

# Overview

This project implements a multithreaded, time-sharing operating system kernel targeting the RISC-V (RV64IMA) architecture.
The kernel is written in C/C++ and Assembly, focusing on thread management (including a scheduler), memory management, synchronization and system call mechanisms.

# Memory Management

Developed a dynamic memory allocator.
Controlled allocation and deallocation of memory blocks.
System calls: 

-mem_alloc 

-mem_free.


# Threads

Implemented a thread scheduler and context switching mechanism. 

Managed per-thread stack allocation and stack initialization with context save/restore for registers during thread switch.

System calls: 

-thread_create

-thread_exit 

-thread_dispatch.

# Semaphores

Implemented semaphores for synchronization, including blocking and wake-up mechanisms for safe coordination between concurrent threads.

System calls: 

-sem_open

-sem_close

-sem_wait

-sem_trywait

-sem_signal.


	
# System Interfaces

ABI (Application Binary Interface)

C API

C++ API

# Technologies

C/C++

RISC-V Assembly

RV64IMA architecture
