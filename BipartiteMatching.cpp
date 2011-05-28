#include "Common.h"
#include "BipartiteMatching.h"

void BipartiteMatchingInit(int argc, char* argv[], Master<pair<bool, int>, int, pair<int, bool> > &master) {
    if (argc != 4) {
        printf ("Usage: %s <num_procs> <partition_mode:s,e,a,l> <input_file>\n", argv[0]);
        exit (1);
    }

    printf("[BipartiteMatchingInit] parsing BipartiteMatching input: %s\n", argv[3]);
    FILE* input = fopen(argv[3], "rt");

    int ret;
    int left_nodes = 0;
    int right_nodes = 0;
    ret = fscanf(input, "%d %d \n", &left_nodes, &right_nodes);
    printf("[BipartiteMatchingInit] file opened: %d left nodes and %d right nodes\n", left_nodes, right_nodes);

    for(int i = 0; i < (left_nodes + right_nodes); i++) {
        // vertex will be added to master when constructed
        BipartiteMatchingVertex *vertex = new BipartiteMatchingVertex(make_pair(i >= left_nodes, NOT_MATCHED), &master);
        int num_links = 0;
        ret = fscanf(input, "%d ", &num_links);
        for(int j = 0; j < num_links; j++) {
            int dest = -1;
            ret = fscanf(input, "%d ", &dest);
            vertex->addOutEdge(dest, 0);
        }
        ret = fscanf(input, "\n");
    }
    fclose(input);
    printf("[BipartiteMatchingInit] finished input parsing\n");
}
