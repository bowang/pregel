
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]){
    if (argc < 4){
        printf ("Usage: %s <left_nodes> <right_nodes> <link_degree> [output_filename]\n", argv[0]);
        exit(1);
    }

    int left_nodes = atoi(argv[1]);
    int right_nodes = atoi(argv[2]);
    int link_degree = atoi(argv[3]);

    char* filename = "bipartitematching.in";
    if (argc > 4)
        filename = argv[4];

    srand(0);

    FILE* output = fopen(filename, "wt");
    fprintf(output, "%d %d \n", left_nodes, right_nodes);
    for(int i = 0; i < left_nodes; i++) {
        int num_links = min(right_nodes, rand()%link_degree);
        fprintf(output, "%d ", num_links);
        for(int j = 0; j < num_links; j++) {
            fprintf(output, "%d ", left_nodes + rand()%right_nodes);
        }
        fprintf(output, "\n");
    }
    for(int i = 0; i < right_nodes; i++) {
        int num_links = min(left_nodes, rand()%link_degree);
        fprintf(output, "%d ", num_links);
        for(int j = 0; j < num_links; j++) {
            fprintf(output, "%d ", rand()%left_nodes);
        }
        fprintf(output, "\n");
    }
    fclose(output);

    return 0;
}
