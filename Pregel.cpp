// Pregel.cpp : Defines the entry point for the console application.

#include "Pregel.h"
#include <sys/time.h>
#include <time.h>

#ifdef PAGERANK
  #include "PageRank.h"
#endif

#ifdef SHORTESTPATH
  #include "ShortestPath.h"
#endif

int main(int argc, char* argv[])
{
    if(argc < 2){
        printf("Usage: %s <num_threads> [application parameters]\n", argv[0]);
        exit(1);
    }
    int numProcs = atoi(argv[1]);

#ifdef PAGERANK
    printf("[Main] running PageRank on Pregel\n");
    Master<double, double, double> master(numProcs);
    PageRankInit(argc, argv, master);
#endif

#ifdef SHORTESTPATH
    printf("[main] running ShortestPath on Pregel\n");
    Master<int, int, int> master(numProcs);
    ShortestPathInit(argc, argv, master);
#endif

    Timer timer;
    timer.start();

    master.initialTasks();
    master.run();

    timer.stop();
    printf("[Main] task done! execution time = %lf\n", timer.get_elapsed_time());
    return 0;
}

