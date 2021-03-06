
/* lab6.c
 * Lab6: Dijkstra.
 * ECE 2230, Fall 2018
 * Zice Wei
 * C73880993
 * This file contains drivers to test the BST package.
 *
 * Purpose and assumptions:This program contains two debug function (works if "-v"):
 *                          a.    res_table_print   // print the shortest path table
 *                          b.    graph_print_array //print the graph matrix
 * No bugs
 *
 * There are three drivers to build a tree using bst_insert and then access
 * keys in the tree using bst_access.  Use
 * -g [1 | 2 | 3 | 4]   Graph type
 * -n N              Number of vertices in graph
 * -a R              Approximate number of adjacent vertices 0 < R < N
 * -h [ 1 | 2 | 3]   Graph operation
 * -s S              Number of the source vertex 0 ≤ S < N
 * -d D              Number of the destination vertex 0 ≤ D < N
 * -v                enable verbose output
 * -r 1234567        seed for the uniform random number genera
 *
 *
 * Verbose and graph_operation will be SENT to graph.c as FLAGS.
 */

 //_XOPEN_SOURCE for drand48 in C99
#define _XOPEN_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <float.h>
#include <limits.h>

#include "graph.h"
//Macro Design
#define TRUE 1
#define FALSE 0
#define SQR(X) ((X)* (X)) //square marco


typedef int bool;

//Global vars for command line paremeters.
int graph_type = -1;
int num_vertices = -1;
int num_adj_vertices = -1;
int graph_operation = -1;
int num_source_vertices = -1;
int num_dest_vertices = -1;
int Seed=0;

bool Verbose = FALSE;

// Local prototype
void getCommandLine(int argc, char **argv);
void graph_add_random(graph_t* G, int link_src, int link_dest, double C, double M);
double weight(int src, int dest);
double distance_g3(Vertex v1, Vertex v2);
analysis random_analysis(graph_t* G, analysis analy );

int main(int argc, char **argv){
    getCommandLine(argc, argv);
    clock_t start, end;
    if(Verbose){
        printf("your input:\n int graph_type=%d\n int num_vertices=%d\n int num_adj_vertices=%d\n int graph_operation=%d\n int num_source_vertices=%d\n int num_dest_vertices=%d\n seed=%d\n", graph_type, num_vertices, num_adj_vertices, graph_operation, num_source_vertices, num_dest_vertices,Seed);
        printf(" verbose=%d\n",Verbose==TRUE?1:0);
    }

    graph_t* G   = NULL;
    PathRes* res = NULL;

    if(graph_type == 1) num_vertices =7;
    if(graph_type == 4) num_vertices =10;

    valid_check(graph_type, num_vertices, num_adj_vertices, graph_operation
                ,num_source_vertices, num_dest_vertices);
    G = construct(num_vertices);

    //type 1
    if( graph_type == 1 ){

        if(Verbose) graph_print_array(G);

        graph_add_edge(G, 1, 2, 3);
        graph_add_edge(G, 1, 6, 5);
        graph_add_edge(G, 2, 3, 7);
        graph_add_edge(G, 2, 4, 3);
        graph_add_edge(G, 2, 6, 10);
        graph_add_edge(G, 3, 4, 5);
        graph_add_edge(G, 3, 5, 1);
        graph_add_edge(G, 4, 5, 6);
        graph_add_edge(G, 5, 6, 7);
        graph_add_edge(G, 6, 0, 4);
        graph_add_edge(G, 6, 3, 8);
        graph_add_edge(G, 6, 4, 9);

    }
    //type 2
    else if(graph_type == 2){
        //add vertex
        for(int link_src = 0; link_src < num_vertices; link_src++){
            for(int link_dest = 0; link_dest < num_vertices; link_dest++){
                if(link_dest == link_src)
                graph_add_edge(G, link_src, link_dest, 0);
                //printf("src:%d dest:%d weight:%f\n",link_src, link_dest, weight(link_src, link_dest));
                graph_add_edge(G, link_src, link_dest, weight(link_src, link_dest ));
            }

        }
    }
    //type 3
    else if(graph_type == 3){

        int R = num_adj_vertices;
        int N = num_vertices;
        double C;
        C = sqrt(R/(N*M_PI));
        double M = log10(1 + SQR(1/C));


        G->vertex[0][0].x = 0.0;
        G->vertex[0][0].y = 0.5;

        G->vertex[N-1][0].x = 1.0;
        G->vertex[N-1][0].y = 0.5;

        srand48(Seed);
        for(int i = 1; i < N-1; i++){
            G->vertex[i][0].x = (double)drand48();
            G->vertex[i][0].y = (double)drand48();
        }


        for(int link_src = 0; link_src < num_vertices; link_src++){
            for(int link_dest = 0; link_dest < num_vertices; link_dest++){
                graph_add_random(G, link_src, link_dest, C, M);
            }

        }
        if(Verbose) graph_print_array(G);

       analysis A;
       A = random_analysis(G, A);
       printf("avg:%f, max:%d, min:%d \n", A.avg, A.max, A.min);

    }

    //type 4
    else if (graph_type == 4){

        graph_add_edge(G, 0, 1, 2);
        graph_add_edge(G, 0, 2, 1);
        graph_add_edge(G, 0, 3, 6);

        graph_add_edge(G, 1, 0, 2);
        graph_add_edge(G, 1, 4, 1);
        graph_add_edge(G, 1, 8, 3);

        graph_add_edge(G, 2, 0, 1);
        graph_add_edge(G, 2, 4, 3);
        graph_add_edge(G, 2, 5, 7);

        graph_add_edge(G, 3, 0, 6);
        graph_add_edge(G, 3, 5, 1);
        graph_add_edge(G, 3, 9, 5);

        graph_add_edge(G, 4, 1, 1);
        graph_add_edge(G, 4, 2, 3);
        graph_add_edge(G, 4, 6, 4);

        graph_add_edge(G, 5, 2, 7);
        graph_add_edge(G, 5, 3, 1);
        graph_add_edge(G, 5, 7, 4);

        graph_add_edge(G, 6, 4, 4);
        graph_add_edge(G, 6, 7, 5);
        graph_add_edge(G, 6, 8, 1);

        graph_add_edge(G, 7, 5, 4);
        graph_add_edge(G, 7, 6, 5);
        graph_add_edge(G, 7, 9, 1);

        graph_add_edge(G, 8, 1, 3);
        graph_add_edge(G, 8, 6, 1);
        graph_add_edge(G, 8, 9, 4);

        graph_add_edge(G, 9, 3, 5);
        graph_add_edge(G, 9, 7, 1);
        graph_add_edge(G, 9, 8, 4);


    }
        if(Verbose){
            printf("after add\n");
            graph_print_array(G);
        }

        if(graph_operation == 1){

            //if(graph_type == 2 || graph_type == 3)
                start = clock();
            res = ShortestPath(G, num_source_vertices);
           // if(graph_type == 2 || graph_type == 3)
                end = clock();

            if (Verbose)res_table_print(G, res, num_source_vertices);
            if(res_find_shortest_path(G, res, num_source_vertices, num_dest_vertices)==-1)
                printf("no such path from %d to %d\n", num_source_vertices, num_dest_vertices);

            res_table_destruct(res);
            res = NULL;
        }
        //Network diameter-- Find max cost.
        else if(graph_operation == 2){
            /*if(graph_type == 2 || graph_type == 3)*/
            start = clock();
            find_max_dest(G);
            /*if(graph_type == 2 || graph_type == 3)*/
            end = clock();


        }
        //Multiple link-disjoint paths
        else if(graph_operation == 3){
            //printf("h=3 nulti path from %d to %d\n", num_source_vertices, num_dest_vertices);
            //if(graph_type == 2 || graph_type == 3)
            start = clock();
            int isDone = 0;
            int time = 0;
            while(isDone==0){
                res = ShortestPath(G, num_source_vertices);
                if(Verbose) res_table_print(G, res, num_source_vertices);
                isDone = res_find_shortest_path(G, res, num_source_vertices, num_dest_vertices);
                res_table_destruct(res);
                res = NULL;
                if (isDone!=-1)time++;
            }
                if(time == 0) printf("no such path from %d to %d\n", num_source_vertices, num_dest_vertices);


            //if(graph_type == 2 || graph_type == 3)
            end = clock();

        }

        graph_destruct(G);

        printf("time = %g\n",1000*((double)(end-start)/CLOCKS_PER_SEC));





    return 0;

}

/* read in command line arguments and store in global variables for easy
 * access by other functions.
 */
void getCommandLine(int argc, char **argv)
{
    /* optopt--if an unknown option character is found
     * optind--index of next element in argv
     * optarg--argument for option that requires argument
     * "x:" colon after x means argument required
     */
    int c;
    int index;
    while ((c = getopt(argc, argv, "r:g:n:a:h:s:d:v")) != -1)
        switch(c) {
            case 'g': graph_type = atoi(optarg);             break;
            case 'n': num_vertices = atoi(optarg);           break;
            case 'a': num_adj_vertices = atoi(optarg);       break;
            case 'h': graph_operation = atoi(optarg);        break;
            case 's': num_source_vertices = atoi(optarg);    break;
            case 'd': num_dest_vertices = atoi(optarg);      break;
            case 'v': Verbose = TRUE;                        break;
            case 'r': Seed = atoi(optarg);                   break;

            case '?':
                if (isprint(optopt))
                    fprintf(stderr, "Unknown option %c.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            default:
                printf("Lab6 command line options\n");

                printf("General options ---------\n");
                printf("-g [1 | 2 | 3 ]  Graph type\n");
                printf(" -n N            Number of vertices in graph\n");
                printf(" -a R            Approximate number of adjacent vertices 0 < R < N\n");
                printf(" -h [ 1 | 2 | 3] Graph operation\n");
                printf(" -s S            Number of the source vertex 0 ≤ S < N\n");
                printf(" -d D            Number of the destination vertex 0 ≤ D < N\n");
                printf(" -v              enable verbose output\n");
                printf(" -r 1234567      seed for the uniform random number genera\n");
                exit(1);
        }
    for (index = optind; index < argc; index++)
        printf("Non-option argument %s\n", argv[index]);
}

/* Cal weight for -g = 2
 * input src. dest
 * return weight.
 */

double weight(int src, int dest){
    return abs(src - dest) + SQR(src - dest + 2) + 3*dest;
}

/* Cal weight for g = 3
 * input i.x i.y. j.x j.y
 * retutn distance
 */
double distance_g3(Vertex v1, Vertex v2){
    return sqrt( SQR(v1.x - v2.x) + SQR(v1.y-v2.y) );
}
/* Add weight for random graph
 * intput graph, link_src, link link_dest , C;
 * no return val.
 */
void graph_add_random(graph_t* G, int link_src, int link_dest, double C, double M){
    double  F, D;
    double weight;
    //CalD(i,j)
    D = (link_src==link_dest)?0:distance_g3(G->vertex[link_src][0], G->vertex[link_dest][0]);

    if(link_src==link_dest)  graph_add_edge(G, link_src, link_dest, 0);
    else if (D <= C ){
        F = log10( 1+ SQR( 1/ ( D+ (C/1000) ) ) );
        weight = M / F;
        graph_add_edge(G, link_src, link_dest, weight);
    }
    else
        graph_add_edge(G, link_src, link_dest, FLT_MAX);
    //Cal
  /*
  if(Verbose){ 
     if(G->vertex[link_src][link_dest].weight!=FLT_MAX){
             printf("X--%d Y--%d | V1.x:%f, V1.y:%f, V2.x:%f,  V2.y:%f -- D:%f, W:%f,  C:%f\n",
             link_src,link_dest, G->vertex[link_src][0].x,G->vertex[link_src][0].y,
                                 G->vertex[link_dest][0].x, G->vertex[link_dest][0].y,
                                 D, weight, C);
             }
      else{

             printf("X--%d Y--%d | V1.x:%f, V1.y:%f, V2.x:%f,  V2.y:%f -- D:%f, W:%s,  C:%f\n",
             link_src,link_dest, G->vertex[link_src][0].x,G->vertex[link_src][0].y,
                                 G->vertex[link_dest][0].x, G->vertex[link_dest][0].y,
                                 D, "FLT_MAX", C);
      }
   
}*/

}

/* Purpose: analysis the avg, max, min adj vertices.
 * input : graph. ayalysys structure.
 *
 */
analysis random_analysis(graph_t* G, analysis A ){
    A.total = 0;
    A.max = INT_MIN;
    A.min = INT_MAX;

    int num_link = 0;
    for(int i = 0; i < G->size; i++){
       num_link = 0;
       for(int j = 0; j < G->size; j++){
           if(G->vertex[i][j].weight != FLT_MAX && i!=j ) num_link++;
       }
           if(num_link!=0){
               A.max = num_link > A.max ? num_link:A.max;
               A.min = num_link < A.min ? num_link:A.min;
           }
       A.total+=num_link;
    }

    A.avg = (A.total)/G->size;
    return A;
}
