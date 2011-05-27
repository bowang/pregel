#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <sys/time.h>

class Timer {

private:
    struct timeval t1, t2;
    double time;
    bool timer_on;

public:
    Timer();
    ~Timer();
    void start();
    void stop();
    double get_elapsed_time();
    
};

#endif

