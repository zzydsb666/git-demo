#ifndef GRAPH_H
#define GRAPH_H

#define MAX_VERTEX_NUM 100
#define INF 32767

typedef struct EdgeNode {
    int adjvex; // The index of the adjacent vertex
    int weight; // Weight of the edge
    struct EdgeNode *next; 
} EdgeNode;

typedef struct VexNode {
    char name[50]; // Name of the vertex
    char info[200]; // Information about the vertex
    EdgeNode *firstedge; // Pointer to the first edge node
} VexNode;

typedef struct {
    VexNode adjlist[MAX_VERTEX_NUM];
    int numVex;
    int numEdge;
} Graph;

void CreateGraph(Graph *G);

int LocateVex(Graph *G, char *name);

void ShowVertex(Graph *G);

void ShowGraph(Graph *G);

void DestroyGraph(Graph *G);

#endif

