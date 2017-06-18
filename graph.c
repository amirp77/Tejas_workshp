#include "test.h"


// create_graph
graph_t *create_graph(int n)
{
    int i;
    graph_t* graph = malloc(sizeof(graph_t));
    graph->nv = n;
 
    graph->array = malloc(n * sizeof(adjlist_t));
 
    for (i = 0; i < n; ++i)
        graph->array[i].head = NULL;
 
    return graph;
}

//create a new node
adj_node_t * newnode(int dest, int weight)
{
    adj_node_t* node =
            (adj_node_t*) malloc(sizeof(adj_node_t));
    node->dest = dest;
    node->weight = weight;
    node->next = NULL;
    return node;
}

void addedge(graph_t* graph, int src, int dest, int weight)
{
#if 1

    adj_node_t * temp2 = graph->array[src].head;
    while(temp2)
    {
        if(temp2->dest == dest)
        {
            return;
        }
        temp2 = temp2 ->next;
    }
#endif

    adj_node_t * temp = newnode(dest, weight);
    temp->next = graph->array[src].head;
    graph->array[src].head = temp;

    // Since graph is undirected, add an edge from dest to src also
    temp = newnode(src, weight);
    temp->next = graph->array[dest].head;
    graph->array[dest].head = temp;
}

void print(graph_t* graph)
{
    int v;
    for (v = 0; v < graph->nv; ++v)
    {
        adj_node_t* temp = graph->array[v].head;
        printf("vetex %d ", v);
        while (temp)
        {
            printf("-> %d wt  = %d ", temp->dest,temp->weight);
            //printf("-> %d", temp->weight);
            temp = temp->next;
        }
        printf("\n");
    }
}
