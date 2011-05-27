#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main(int argc, char* argv[]){
    if(argc < 3){
        printf("Usage: %s <num_sites> <link_degree> [output_filename]\n", argv[0]);
        exit(1);
    }

    int num_sites = atoi(argv[1]);
    int link_degree = atoi(argv[2]);
    char* filename = "pagerank.in";
    if(argc > 3)
        filename = argv[3];
    srand(0);
    
    FILE* output = fopen(filename,"wt");
    fprintf(output, "%d\n", num_sites);
    for(int i = 0; i < num_sites; ++i){
        int num_links = rand()%link_degree;
        fprintf(output, "%d ", num_links);
        for(int j = 0; j < num_links; ++j){
            fprintf(output, "%d ", rand()%num_sites );
        }
        fprintf(output, "\n");
    }
    fclose(output);

    return 0;
}

