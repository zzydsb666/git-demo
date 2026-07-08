#ifndef GRAPH_H
#define GRAPH_H
#include "global.h"

typedef struct CNode {
    int index;
    int length;
    struct CNode* next;
} CNode;

typedef struct{
    VertexType roadlist[MAX_VERTEX_NUM];
    CNode* edge_head[MAX_VERTEX_NUM];
    int nodenum;
    int edgenum;
} ALGraph;

void InitGraph(ALGraph *G);
void CreateGraph(ALGraph *G);
int LocateVertex(ALGraph *G,VertexType name);
void ShowGraph(ALGraph *G);

#endif