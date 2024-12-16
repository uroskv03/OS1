
#include "../h/Sem.hpp"
#include "../h/syscall_c.hpp"


int Sem::sem_close() {
    if(closed) return -1;
    closed = true;
    while (blocked.peekFirst() != nullptr){
        deblock();
    }
    return 0;
}

int Sem::wait() {
    if(closed) return -1;
    if(--val <0) block();
    if(closed) return -1;
    return 0;
}

int Sem::sem_signal() {
    if(closed) return -1;
    if(++val <= 0){
        deblock();
    }
    return 0;
}

int Sem::tryWait() {
    if(closed) return -1;
    if(val<=0) return -1;
    if(--val == 0) {
        return 0;
    }
    return 1;
}

void Sem::block() {
    blocked.addFirst(TCB::running);
    TCB::running->setBlocked(true);
    br++;
    thread_dispatch();
}


void Sem::deblock() {
    if(br<=0) return;
    blocked.peekFirst()->setBlocked(false);
    Scheduler::put(blocked.peekFirst());
    blocked.removeFirst();
    br --;
}

Sem::~Sem() {
    this->sem_close();
}



