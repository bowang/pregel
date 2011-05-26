#include <fstream>
#include "PageRank.h"

void PageRankInit(int argc, char* argv[], Master<double, double, double> &master){
    if (argc != 2) {
        printf ("Usage: pagerank <input file>\n");
        exit (1);
    }
    
    printf("[PageRankInit] parsing PageRank input: %s\n", argv[1]);
    
    FILE* input = fopen(argv[1], "rt");
    
    int num_sites = 0;
    fscanf(input, "%d\n", &num_sites);
    printf("[PageRankInit] file opened: %d sites all together\n", num_sites);
    for(int i = 0; i < num_sites; ++i){
        // vertex will be added to master when constructed
        PageRankVertex *vertex = new PageRankVertex(1.f, &master);
        int num_links = 0;
        fscanf(input, "%d ", &num_links);
        for(int j = 0; j < num_links; ++j){
            int dest = -1;
            fscanf(input, "%d ", &dest);
            vertex->addOutEdge(dest, 1.f);
        }
        fscanf(input, "\n");
    }
    fclose(input);
    printf("[PageRankInit] finished input parsing\n");
}
