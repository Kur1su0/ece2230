
 /* graph.c
  * Lab6: Dijkstra.
  * ECE 2230, Fall 2018
  * Zice Wei
  * C73880993
  * Purpose and assumptions: This file contains Dijkstra Algorithms.
  * bugs: there is no bugs
  * This program contains two debug function (works if "-v"):
  *                      a.    res_table_print   // print the shortest path table
  *                      b.    graph_print_array //print the graph matrix
  * Verbose and graph_operation will be SENT to graph.c as FLAGS.
  */
#include <limits.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph.h"

//external int for args flag.
extern int Verbose;
extern int graph_operation;

//ptototype
void find_shortest_path_rec(graph_t* G, PathRes* res, int source,int dest ,int current);
/* Purpose construct a graph
 * input: int of number of vertices.
 * return: double pointer for a graph.
 *         NULL if size is not correct;
 *
 *
 */
 // G->vertx[i][j] = ?;
graph_t* construct(int size){
    if(size == 0) return NULL;

    graph_t* G = (graph_t*)calloc(1,sizeof(graph_t));
    G->size = size;
    G->vertex = (Vertex**)calloc(size, sizeof(Vertex*) );
    int i =0, j = 0;
    for(i = 0; i < size; i++)
        G->vertex[i] = (Vertex*)calloc(size, sizeof(Vertex));
    //initialize each vertex.
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            G->vertex[i][j].weight = (i==j) ? 0:FLT_MAX;
            //G->vertex[i][j].confirm = FALSE;
        }
    }
    return G;
}

/* destruct a graph
 * intput a pointer to a graph, size of vertices.
 * no return val.
 *
 */

void graph_destruct(graph_t* G){
   int size = G->size;
   for(int i = 0; i < size; i++){
       free(G->vertex[i]);
       G->vertex[i] = NULL;
   }
   free(G->vertex);
   G->vertex = NULL;

   free(G);
}

/* Print tag
 * input size;
 * return 1 .
 */
int print_row_tag(int size){
    printf("%-9s"," ");
    for (int k=0; k< size; k++){
        printf("%-9d",k);
        }
    printf("\n");
    return 1;
}

/* Print tag
 * input size;
 * no return val .
 */
void print_col_tag(int size, int curr){
    if(curr < size) printf ("%-9d", curr);

}

/* Print the elets in the 2d array.
 * input pointer to a graph, size of graph
 * no return val.
 *
 */
void graph_print_array(graph_t* my_graph){
    int rFlag=0;
    Vertex** G = my_graph->vertex;
    int size = my_graph->size;
    if(rFlag !=1 ) rFlag = print_row_tag(size);

    for (int i=0; i < size; i++){
        print_col_tag(size, i);

        for(int j=0; j < size; j++){

           if(i==j)                    printf("%-9d", 0 );
           else if(G[i][j].weight == FLT_MAX) printf("%-9s", "INF");
           else                        printf("%-9f", G[i][j].weight);
        }
        printf("\n");
    }
    printf("\n");

 }

/*  Add vertex weight to the graph
 *  input: pointer to a graph, lnk_src, lnk_dest, weight.
 */
void graph_add_edge(graph_t* G, int src, int dest, double weight){
     G->vertex[src][dest].weight = weight;
 }

/*  Add vertex distance to the graph
 *  input: pointer to a graph, lnk_src, lnk_dest, distance.
 */
void graph_add_distance(graph_t* G, int src, int dest, double distance){
     G->vertex[src][dest].weight = distance;
}

PathRes* PathRes_init(int size){
    PathRes* PathRes_ptr = (PathRes*)calloc(size, sizeof(PathRes));
    assert(PathRes_ptr != NULL);
    return PathRes_ptr;
}
/* Inputs:
 *     source:  the source vertex in the set V of vertices
 *     G:       a graph data structure that includes:
 *         a.  count of number of vertices in the set V
 *         b.  adjacency matrix.
 *
 * Outputs:
 *     Res:
 *     a structure for the path cost to each vertex from the
 *                     source on the shortest path
 *     and  for the predecessor vertex (or the upsteam
 *                     neighbor) on the shortest path to each vertex
 */

PathRes* ShortestPath(graph_t* G, int source)
{
    int size = G->size;
    int count_added = 1;   // for verbose prints
    if (Verbose) {
        printf("Starting at node %d\n", source);
    }
    // W is a set indicating which vertices have been confirmed, that is,
    //   the vertices for which we have found the shortest distance

    // Initialize W and ShortestDistance[u] as follows:
    confirm_array* W = (confirm_array*)calloc(1, sizeof(confirm_array));
    W->total   = G->size;
    W->current = 0;
    W->confirm = (int*)calloc(G->size, sizeof(int));

    for(int i=0; i < size; i++ ) W->confirm[i] = FALSE;
    W->confirm[source] = TRUE;
    W->current++;


    //for (each u in V - {source} ) {
    //    ShortestDistance[u] = T[source][u];
    //}
    PathRes* res = PathRes_init(G->size);
    for (int i=0; i < G->size; i++) {
        res[i].dist = G->vertex[source][i].weight;
        res[i].prede = source;
    }
    res[source].dist = 0;
    res[source].prede  = INT_MAX;
    

    // repeatedly enlarge W until W includes all vertices in V
    while (W->current != W->total) {

        // find the vertex w in V - W at the minimum distance from source
        double MinDistance = FLT_MAX;
        int w = INT_MAX;
        //for (each v in V - W) {
        for(int v = 0; v < size; v++ ) {
            if (W->confirm[v] == TRUE){
                continue;
            }
            if ( (res[v].dist!=0) && (res[v].dist < MinDistance)  ) {
              //printf("%d -  cost:%f\n",v,res[v].dist);
                MinDistance = res[v].dist;
                w = v;
            }
        }

        // if a vertex w was found, add w to W, the confirmed set
        //W = W union {w};
        if (w != INT_MAX){
            W->confirm[w] = TRUE;
            W->current++;
            //w is the succ node of source
            if (Verbose) {
                count_added++;
                printf(" %d added with cost %g\n", w, MinDistance);
            }


        // update the shortest distances to vertices in V - W via w
        //for (each u in V - W) {
          for (int u = 0; u < size; u++ ){
            // if vertex u is a neighbor of w
            if (G->vertex[w][u].weight < FLT_MAX) {
               double cost_to_u_via_w = res[w].dist + G->vertex[w][u].weight;

                if (cost_to_u_via_w < res[u].dist) {
                    res[u].dist = cost_to_u_via_w;
                    res[u].prede = w;
                    if (Verbose) {
                        printf("\t%d has lower cost %g with predecessor %d\n" , u, cost_to_u_via_w, w);
                    }
                }
            }
          }

        }
        else{
            if (Verbose)  printf("no valid vertex\n");
            break;
        }
    }
    if (Verbose) {
        printf("Found %d nodes, including source\n", count_added);
    }
    free(W->confirm);
    free(W);
    return res;
}
/* Depug print table.
 * input pointer to a Graph, graph table and index of the source
 *
 * no return val.
 */
void res_table_print(graph_t* G, PathRes* res, int source){
        printf("table for source:%d\n", source);
        printf("%-9s%-9s%-9s\n","vertex","dist","prede");
        int* list = (int*)calloc(G->size,sizeof(int));
        int min_index = 0;
        int count = 0;
        double min = res[0].dist;
        while(count!=G->size){
            for(int i = 0; i < G->size; i++ ){
                if (list[i] != 1){
                    if( min > res[i].dist){
                        min = res[i].dist;
                        min_index = i;
                    }
                }
            }

                //printf("%-9d %-9f %-9d\n",count, res[count].dist,res[count].prede);

                count++;

                list[min_index] = 1;
                print_col_tag(G->size,min_index);

                if(res[min_index].prede == INT_MAX )
                    printf("%-9f %-9s",res[min_index].dist,"s");

                else if(res[min_index].dist == FLT_MAX ) continue;

                else
                    printf("%-9f %-9d",res[min_index].dist,res[min_index].prede);

                printf("\n");
                for(int i = 0; i < G->size;i++){
                    if(list[i]!=1){
                        min_index = i;
                        min = res[min_index].dist;
                    }
                }


       }
       free(list);
       list = NULL;
}

/* destruct  a res table.
 * input res table
 * no return val
*/
void res_table_destruct(PathRes* res){
     free(res);
     res=NULL;

}

/*
 *find a shoertest path recursively
 *input pointer to a graph, res table, and int source, dest , current vertex
 *no return val
*/
void find_shortest_path_rec(graph_t* G, PathRes* res, int source,int dest ,int current){
    if(res[current].prede == INT_MAX){
        printf("%d", current);
        return;
    }else{
        find_shortest_path_rec(G, res, source, dest, res[current].prede);
        if( graph_operation == 3){
                G->vertex[current][res[current].prede].weight = FLT_MAX;
                G->vertex[res[current].prede][current].weight = FLT_MAX;
        }
        printf(" -- %d", current);

    }

}

/* purpose: find the short path from source to dest.
 *
 * input  : graph, res table, source index, dest index
 * return : -1 : not found
 *           0 : there are some vertices bewteen 2 vertices
 *           1 :   "" are not  ""
 */
int res_find_shortest_path(graph_t *G,  PathRes* res, int source, int dest){
    if(res[dest].dist == FLT_MAX || res[dest].prede == INT_MAX){
       //printf("no such path from %d to %d\n", source ,dest);

        return -1;
    }
    printf("coost %f : ", res[dest].dist);
    //printf("cost %f: ", max.cost);
    find_shortest_path_rec(G, res, source, dest,  dest);
    printf("\n");
    //if(res[dest].prede == source) return 1;
    return 0;

}
/*check args validation.
 *input: graph_type, num_vertices, num_adj_vertices, graph_operation
 *               num_source_vertices, num_dest_vertices){
 * no return val/
 */
void valid_check(int g, int n, int a, int h, int s, int d){
    if(g == 2){
        if(n>=0);
        else{
            printf("error args: graph type2: n >= 0\n");
            exit(1);
        }
        //assert(n>=0);
    }

    if(g == 3){
        if(a>0 && a<n);
        else{
            printf("error args: graph type3:  0 < a < N\n");
            exit(1);
        }
        //assert(a>0 && a < n);

    }
    if(h == 1 || h == 3){
        if((s>=0 && s<n) && (d>=0 && d<n) );
        else{
            printf("error args: graph operation%d: 0 <= s < n  and 0 <= d < n\n",h);
            exit(1);
        }
        //assert((s>=0 && s<n) && (d>=0 && d<n) );
    }
}


/* Find path with max cost.
 * input pointer to the graph
 *
 */

void find_max_dest(graph_t* G){
    max_cost max;
    int flag = 0;
    int size = G->size;
    PathRes* res = NULL;
    //Start from every vertex to find res.
    for(int i = 0; i < G->size; i++){
       res = ShortestPath(G, i);
       //in each res table
       for(int j = 0; j < size; j++){
           if(res[j].dist == FLT_MAX) continue;

           if (flag==0){
               max.s = i;
               max.d = j;
               max.cost = res[j].dist;
               flag = 1;
           }
           if(res[j].dist > max.cost){
               max.s = i;
               max.d = j;
               max.cost = res[j].dist;
           }
       }

        res_table_destruct(res);
        res = NULL;
    }

    res = ShortestPath(G, max.s);
    if(Verbose) res_table_print(G, res, size);
    res_find_shortest_path(G, res, max.s, max.d);

    res_table_destruct(res);
    res = NULL;

}
