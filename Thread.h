#ifndef THREAD_H
#define THREAD_H

#include "Common.h"

class Thread {
public:
    Thread(){}
    void start();
    void join();
    virtual void* run() = 0;
    void* exitCode();

private:
    static void* threadRouter(void* arg);
    void executeThread();
    pthread_t _pthread;
    void* _exitCode;
};


#endif
