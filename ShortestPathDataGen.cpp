#include <stdlib.h>
#include <stdio.h>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]){
    if (argc < 4){
        printf ("Usage: %s <num_nodes> <link_degree> <max_dist> [output_filename]\n", argv[0]);
        exit(1);
    }

    int num_nodes = atoi(argv[1]);
    int link_degree = min(atoi(argv[2]), num_nodes-1);
    int max_dist = atoi(argv[3]);
    char* filename = "shortestpath.in";
    if (argc > 4)
        filename = argv[4];

    srand(0);

    FILE* output = fopen(filename, "wt");
    fprintf(output, "%d\n", num_nodes);
    for(int i = 0; i < num_nodes; i++) {
        int num_links = rand()%link_degree;
        fprintf(output, "%d ", num_links);
        for(int j = 0; j < num_links; j++) {
            int node = rand()%num_nodes;
            while(node == i)
                node = rand()%num_nodes;
            fprintf(output, "%d %d ", node, rand()%max_dist);
        }
        fprintf(output, "\n");
    }
    fclose(output);
    
    return 0;
}
