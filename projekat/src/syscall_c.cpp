#include "../h/syscall_c.hpp"

//#include "../h/MemoryAllocator.hpp"
//#include "../lib/console.h"
//#include "../lib/mem.h"

void *mem_alloc(size_t size) {
    size = ((size+MemoryAllocator::header_size)/MEM_BLOCK_SIZE + ((size+MemoryAllocator::header_size) % MEM_BLOCK_SIZE != 0));
    __asm__ volatile("mv a1, %0" : : "r" (size));
    __asm__ volatile("li a0, 0x01");
    __asm__ volatile("ecall");
    return (void*) Riscv::r_a0();
}

int mem_free(void * p) {
    __asm__ volatile("mv a1, %0" : : "r" (p));
    __asm__ volatile("li a0, 0x02");
    __asm__ volatile("ecall");
    return (int) Riscv::r_a0();
}

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg) {
    void* stack_space = mem_alloc(DEFAULT_STACK_SIZE);
    if(stack_space == nullptr){
        return -1;
    }
    __asm__ volatile("mv a7, %0" : : "r" (uint64(stack_space)+DEFAULT_STACK_SIZE));
    __asm__ volatile("mv a6, %0" : : "r" (arg));
    __asm__ volatile("mv a2, %0" : : "r" (start_routine));
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("li a0, 0X11");
    __asm__ volatile("ecall");
    return (int) Riscv::r_a0();
}

int thread_exit() {
    __asm__ volatile("li a0, 0x12");
    __asm__ volatile("ecall");
    return (int) Riscv::r_a0();
}

void thread_dispatch() {
    __asm__ volatile("li a0, 0x13");
    __asm__ volatile("ecall");
}

int sem_open(sem_t *handle, unsigned int init) {
    if(handle == nullptr) return -1;
    __asm__ volatile("mv a2, %0" : : "r" (init));
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("li a0, 0x21");
    __asm__ volatile("ecall");
    return (int) Riscv::r_a0();
}

int sem_close(sem_t handle) {
    if(handle == nullptr) return -1;
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("li a0, 0x22");
    __asm__ volatile("ecall");
    return (int) Riscv::r_a0();
}

int sem_wait(sem_t id) {
    if(id == nullptr) return -1;
    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("li a0, 0x23");
    __asm__ volatile("ecall");
    return (int) Riscv::r_a0();
}

int sem_signal(sem_t id) {
    if(id == nullptr) return -1;
    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("li a0, 0x24");
    __asm__ volatile("ecall");
    return (int) Riscv::r_a0();
}

int sem_timedwait(sem_t id, time_t timeout) {
    if(id == nullptr) return -1;
    __asm__ volatile("mv a2, %0" : : "r" (timeout));
    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("li a0, 0x25");
    __asm__ volatile("ecall");
    return (int) Riscv::r_a0();
}

int sem_trywait(sem_t id) {
    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("li a0, 0x26");
    __asm__ volatile("ecall");
    return (int) Riscv::r_a0();
}

int time_sleep(time_t time) {
    __asm__ volatile("mv a1, %0" : : "r" (time));
    __asm__ volatile("li a0, 0x31");
    __asm__ volatile("ecall");
    return (int) Riscv::r_a0();
}

char getc() {
    __asm__ volatile("li a0, 0x41");
    __asm__ volatile("ecall");
    return (int) Riscv::r_a0(); //mozda treba i konvert u int zbog EOF-a
}

void putc(char c) {
    __asm__ volatile("mv a1, %0" : : "r" (c));
    __asm__ volatile("li a0, 0x42");
    __asm__ volatile("ecall");
}

/*
void prfree() {
    __asm__ volatile("li a0, 0x51");
    __asm__ volatile("ecall");
}*/