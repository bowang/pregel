#ifndef COMMON_H
#define COMMON_H
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <vector>
#include <map>
#include <set>
#include "pthread.h"
#include "Timer.h"

using namespace std;

#ifdef DPRINT
    #define PRINTF(...) printf(__VA_ARGS__)
#else
    #define PRINTF(...) 
#endif

#ifdef DPRINT
    #define FPRINTF(...) fprintf(__VA_ARGS__)
#else
    #define FPRINTF(...) 
#endif

#endif
