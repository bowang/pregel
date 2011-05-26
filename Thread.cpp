#include "Thread.h"

void Thread::start(){
    printf("[Thread] start\n");
    pthread_create(&_pthread, NULL, Thread::threadRouter, reinterpret_cast<Thread*>(this));
}

void Thread::join(){
    pthread_join(_pthread, NULL);
}

void* Thread::threadRouter(void* arg){
    printf("[Thread] threadRouter\n");
    reinterpret_cast<Thread*>(arg)->executeThread();
    return NULL;
}

void Thread::executeThread(){
    printf("[Thread] executeThread\n");
    _exitCode = run();
}

void* Thread::exitCode(){
    return _exitCode;
}
