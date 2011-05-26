#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <vector>
#include <map>
#include <set>
#include "pthread.h"

using namespace std;

#ifdef DPRINT
    #define PRINTF(...) printf(__VA_ARGS__)
#else
    #define PRINTF(...) 
#endif

#endif
