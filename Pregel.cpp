// Pregel.cpp : Defines the entry point for the console application.

#include "Pregel.h"

#ifdef PAGERANK
  #include "PageRank.h"
#endif

int main(int argc, char* argv[])
{
    if(argc < 2){
        printf("Usage: %s <num_threads> [application parameters]\n", argv[0]);
        exit(1);
    }
    int numProcs = atoi(argv[1]);
    Master<double, double, double> master(numProcs);

#ifdef PAGERANK
    printf("[main] running PageRank on Pregel\n");
    PageRankInit(argc, argv, master);
#endif

    master.initialTasks();
    master.run();
    printf("[main] task done!\n");
    return 0;
}

