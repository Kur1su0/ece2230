/* graph.h
 * Lab6: Dijkstra.
 * ECE 2230, Fall 2018
 * Zice Wei
 * C73880993
 * Purpose and assumptions: This file contains structures and function prototypes
 *
 * 
 * No bugs
 */

#define TRUE 1
#define FALSE 0
typedef int bool;
// Matrix desinged.
typedef struct Vertex{
  double weight;
  double distance;
  double x;
  double y;
  //bool confirm;

}Vertex;
typedef struct graph_t_tag{
    int size;
    Vertex** vertex;
}graph_t;


typedef struct PathRes{
    double dist;
    int prede;
}PathRes;


typedef struct confirm_array{
    int total;
    int current;
    int* confirm;

}confirm_array;

typedef struct max_cost{
    int    s;
    int    d;
    double cost;
}max_cost;

typedef struct analysis{
    int max;
    int min;
    double avg;
    int total;
}analysis;


//prototype
void valid_check(int g, int n, int a, int h, int s, int d);
graph_t* construct(int size);
void graph_destruct(graph_t* G);
void print_col_tag(int size, int curr);
void graph_print_array(graph_t* G);
void graph_add_edge(graph_t* G, int src, int dest, double weight);
void graph_add_distance(graph_t* G, int src, int dest, double distance);


PathRes* ShortestPath(graph_t* G, int source);
void res_table_print(graph_t* G, PathRes* res, int source);
void res_table_destruct(PathRes* res);
int  res_find_shortest_path(graph_t* G, PathRes* res, int source, int dest);
void find_max_dest(graph_t* G);
//void find_multi_path(graph_t* G);
