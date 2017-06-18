#ifndef GRAPH_ACT
#define GRAPH_ACT

#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <signal.h>

struct my_file{
    FILE *fptr;
    pthread_mutex_t flock;
};
typedef struct my_file my_file_t;

struct node{
    int dest;
    int wt;
};
typedef struct node node_t;

struct gnode{
    int vertex;
    int nedges;
    node_t **node;
};
typedef struct gnode gnode_t;

struct adj_node
{
    int weight;
        int dest;
        struct adj_node* next;
};
typedef struct adj_node adj_node_t;

struct adjlist
{
        adj_node_t *head;  // pointer to head node of list
};
typedef struct adjlist adjlist_t;

struct graph
{
        int nv;
        adjlist_t * array;
};

typedef struct graph graph_t;

struct thread_data{
    pthread_t id;
    int vertex;
    int edges;
};

graph_t *create_graph(int n);
adj_node_t * newnode(int dest, int weight);
void addedge(graph_t* graph, int src, int dest, int weight);
void print(graph_t* graph);

#define SUCCESS 0
#define FAILURE -1
#endif

