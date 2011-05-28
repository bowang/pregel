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

#ifdef BIPARTITEMATCHING
  #include "BipartiteMatching.h"
#endif

int main(int argc, char* argv[])
{
    if(argc < 2){
        printf("Usage: %s <num_threads> <partition_mode:s,e,a,l> [application parameters]\n", argv[0]);
        exit(1);
    }
    int numProcs = atoi(argv[1]);
    PartitionHeuristics partitionMode;
    switch(argv[2][0]) {
        case 's': partitionMode = SimplePartition; break;
        case 'e': partitionMode = EvenPartition; break;
        case 'a': partitionMode = AdaptivePartition; break;
        case 'l': partitionMode = LocalityPartition; break;
        default : printf("[Main] unknown partition mode\n"); exit(1);
    };

#ifdef PAGERANK
    printf("[Main] running PageRank on Pregel\n");
    Master<double, double, double> master(numProcs, partitionMode);
    PageRankInit(argc, argv, master);
#endif

#ifdef SHORTESTPATH
    printf("[main] running ShortestPath on Pregel\n");
    Master<int, int, int> master(numProcs, partitionMode);
    ShortestPathInit(argc, argv, master);
#endif

#ifdef BIPARTITEMATCHING
    printf("[main] running BipartiteMatching on Pregel\n");
    Master<pair<bool, int>, int, pair<int, bool> > master(numProcs, partitionMode);
    BipartiteMatchingInit(argc, argv, master);
#endif

    Timer timer;
    timer.start();

    master.initialTasks();
    master.run();

    timer.stop();
    printf("[Main] task done! execution time = %lf\n", timer.get_elapsed_time());
    return 0;
}

