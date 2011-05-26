// Pregel.cpp : Defines the entry point for the console application.

#include "Pregel.h"

#ifdef PAGERANK
  #include "PageRank.h"
#endif

int main(int argc, char* argv[])
{
    Master<double, double, double> master;
    master.initialTasks();

#ifdef PAGERANK
    printf("[main] running PageRank on Pregel\n");
    PageRankInit(argc, argv, master);
#endif

    master.run();
    printf("[main] task done!\n");
    return 0;
}

