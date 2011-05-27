#include <stdio.h>
#include <cstdlib>
#include "Timer.h"

Timer::Timer() {
    time = 0.0f;
    timer_on = false;
}

Timer::~Timer() {

}

void Timer::start(){
    if(timer_on==true){
        printf("Timer Error: start() and stop() unmatched.\n");
        exit(1);
    }
    else{
        timer_on = true;
        gettimeofday(&t1, NULL);
    }
}

void Timer::stop(){
    if(timer_on==false){
        printf("Timer Error: start() and stop() unmatched.\n");
        exit(1);
    }
    else{
        timer_on = false;
        gettimeofday(&t2, NULL);
        time += (double(t2.tv_sec)*1000000.0f + double(t2.tv_usec)) 
              - (double(t1.tv_sec)*1000000.0f + double(t1.tv_usec));
    }
}

double Timer::get_elapsed_time() {
    return time/1000000.0f;
}
