#include "Common.h"
#include "ShortestPath.h"

#define INF 2147483647

void ShortestPathInit(int argc, char* argv[], Master<int, int,int> &master){
    if (argc != 3) {
        printf ("Usage: %s <num_procs> <input_file>\n", argv[0]);
        exit (1);
    }

    printf("[ShortestPathInit] parsing ShortestPath input: %s\n", argv[2]);
    FILE* input = fopen(argv[2], "rt");

    int ret;
    int num_nodes = 0;
    ret = fscanf(input, "%d\n", &num_nodes);
    printf("[ShortestPathInit] file opened: %d nodes all together\n", num_nodes);
    for(int i = 0; i < num_nodes; i++) {
        // vertex will be added to master when constructed
        ShortestPathVertex *vertex = new ShortestPathVertex(INF, &master);
        int num_links = 0;
        ret = fscanf(input, "%d ", &num_links);
        for(int j = 0; j < num_links; j++) {
            int dest = -1;
            int edge_value = -1;
            ret = fscanf(input, "%d %d ", &dest, &edge_value);
            vertex->addOutEdge(dest, edge_value);
        }
        ret = fscanf(input, "\n");
    }
    fclose(input);
    printf("[ShortestPathInit] finished input parsing\n");
}
