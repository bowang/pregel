#include "Common.h"
#include "PageRank.h"

void PageRankInit(int argc, char* argv[], Master<double, double, double> &master){
    if (argc != 3) {
        printf ("Usage: %s <num_procs> <input_file>\n", argv[0]);
        exit (1);
    }
    
    printf("[PageRankInit] parsing PageRank input: %s\n", argv[2]);
    FILE* input = fopen(argv[2], "rt");
    
    int ret;
    int num_sites = 0;
    ret = fscanf(input, "%d\n", &num_sites);
    printf("[PageRankInit] file opened: %d sites all together\n", num_sites);
    for(int i = 0; i < num_sites; ++i){
        // vertex will be added to master when constructed
        PageRankVertex *vertex = new PageRankVertex(1.f, &master);
        int num_links = 0;
        ret = fscanf(input, "%d ", &num_links);
        for(int j = 0; j < num_links; ++j){
            int dest = -1;
            ret = fscanf(input, "%d ", &dest);
            vertex->addOutEdge(dest, 1.f);
        }
        ret = fscanf(input, "\n");
    }
    fclose(input);
    printf("[PageRankInit] finished input parsing\n");
}
