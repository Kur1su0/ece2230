#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define TRUE 1
#define FALSE 0
typedef int bool;
// Matrix desinged.
typedef struct Vertex{
  double weight;
  bool isComfirm;

}Vertex;

typedef struct graph_t_tag{
    int size;
    Vertex** vertex;
}graph_t;


typedef struct PathRes{
    double dist;
    double Prede;
}PathRes;


int main(){

graph_t * G = (graph_t*)calloc(1, sizeof(graph_t));

    G->size = 7;
    int size = G->size;
    
    G->vertex = (Vertex**)calloc(size, sizeof(Vertex*) );
    int i =0, j = 0;
    for(i = 0; i < size; i++)
        G->vertex[i] = (Vertex*)calloc(size, sizeof(Vertex));
    //initialize each vertex.
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            G->vertex[i][j].weight = i+j;
        }
    }
    G->vertex[0][3].weight = 5;
    G->vertex[0][0].isComfirm = TRUE;
    
    printf("G->vertex[6][6].weight=%f\nG->vertex[0][0].isComfirm = %d\n", 
              G->vertex[6][6].weight,  G->vertex[0][0].isComfirm);


return 0;
}
