# Kernel- Operating system

# Overview

This project implements a multithreaded, time-sharing operating system kernel targeting the RISC-V (RV64IMA) architecture.
The kernel is written in C/C++ and Assembly, focusing on thread management (including a scheduler), memory management, synchronization and system call mechanisms.

# Memory Management

Developed a dynamic memory allocator.

Controlled allocation and deallocation of memory blocks with functions: mem_alloc and mem_free.


# Threads

Implemented a thread scheduler and contex switching mechanism with functions: thread_create, thread_exit and thread_dispatch. 

# Semaphores

Implemented semaphores for synchronization with blocking and wake-up mechanisms with safe coordination between concurrent threads with functions: sem_open, sem_close, sem_wait, sem_trywait and sem_signal.


# System Interfaces

ABI (Application Binary Interface)

C API

C++ API

# Technologies

C/C++

RISC-V Assembly

RV64IMA architecture
