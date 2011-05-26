#include "Thread.h"

void Thread::start(){
    pthread_create(&_pthread, NULL, Thread::threadRouter, reinterpret_cast<Thread*>(this));
}

void Thread::join(){
    pthread_join(_pthread, NULL);
}

void* Thread::threadRouter(void* arg){
    reinterpret_cast<Thread*>(arg)->executeThread();
    return NULL;
}

void Thread::executeThread(){
    _exitCode = run();
}

void* Thread::exitCode(){
    return _exitCode;
}
