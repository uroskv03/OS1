
#include "../h/syscall_cpp.hpp"
#include "../lib/mem.h"


using size_t = decltype(sizeof(0));
void* operator new (size_t size) {
    return mem_alloc(size);
}

void operator delete (void* p) {
    mem_free(p);
}

/*void* operator new[] (size_t size) {
    return mem_alloc(size);
}

void operator delete[] (void* p){
    mem_free(p);
}
*/
Thread::Thread(void (*body)(void *), void *arg) {
    //this->myHandle = nullptr;
    this->body = body;
    this->arg = arg;
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::start() {
    return thread_create(&myHandle, body , arg);
}

int Thread::sleep(time_t t)  {
    return time_sleep(t);
}

Thread::~Thread() {
}

Thread::Thread() {
    this->body = &runWrapper;
    this->arg = this;
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::timedWait(time_t t) {
    return sem_timedwait(myHandle,t);
}

int Semaphore::tryWait() {
    return sem_trywait(myHandle);
}

Semaphore::~Semaphore() {
    delete this->myHandle;
}

char Console::getc() {
    return __getc();
}

void Console::putc(char c) {
    __putc(c);
}
