#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int LocateVex(Graph *G, char *name) {
    int i;
    for (i = 0; i < G->numVex; i++) {
        if (strcmp(G->adjlist[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void CreateGraph(Graph *G) {
    int i, startIdx, endIdx, w, k;
    char start[20], end[20];
    EdgeNode *p;
    char line[100];
    printf("请输入图信息：");
    printf("请输入顶点数；");
    fgets(line, sizeof(line), stdin);
    if (sscanf(line, "%d", &G->numVex) != 1 || G->numVex <= 0 || G->numVex > MAX_VERTEX_NUM) {
        printf("输入的顶点数无效，请输入一个正整数且不超过 %d。\n", MAX_VERTEX_NUM);
        G->numVex = 0;
        G->numEdge = 0;
        return;
    }



printf("请输入边数：");
    fgets(line, sizeof(line), stdin);
    if (sscanf(line, "%d", &G->numEdge) != 1 || G->numEdge <= 0 || G->numEdge > MAX_VERTEX_NUM * (MAX_VERTEX_NUM - 1) / 2) {
        printf("输入的边数无效，请输入一个正整数且不超过 %d。\n", MAX_VERTEX_NUM * (MAX_VERTEX_NUM - 1) / 2);
        G->numEdge = 0;
        return;
    }
// Initialize the adjacency list
    printf("录入所有顶点信息：\n");
    for (i = 0; i < G->numVex; i++) {
        printf("请输入第 %d 个顶点的名称：", i + 1);
        fgets(G->adjlist[i].name, sizeof(G->adjlist[i].name), stdin);
        G->adjlist[i].name[strcspn(G->adjlist[i].name, "\n")] = '\0'; // Remove newline character
        printf("请输入景点简介：");
        fgets(G->adjlist[i].info, 50, stdin);
        G->adjlist[i].info[strcspn(G->adjlist[i].info, "\n")] = '\0'; // Remove newline character
        G->adjlist[i].firstedge = NULL;
    }

    printf("录入景点连通道路及权值：\n");
    for (k = 0; k < G->numEdge; k++) {
        printf("输入第 %d 条边的起点和终点名称及权值（格式：起点 终点 权值）：", k + 1);
        fgets(line, sizeof(line), stdin);
        if (sscanf(line, "%s %s %d", start, end, &w) != 3 || w <= 0) {
            printf("输入格式错误或权值无效，请重新输入。\n");
            k--; // Decrement k to retry this edge
            continue;
        }
        startIdx = LocateVex(G, start);
        endIdx = LocateVex(G, end);
        if (startIdx == -1 || endIdx == -1) {
            printf("输入的顶点名称无效，请重新输入。\n");
            k--; // Decrement k to retry this edge
            continue;
        }
        p = (EdgeNode *)malloc(sizeof(EdgeNode));
        if (p == NULL) {
            printf("内存分配失败！\n");
            exit(1);
        }
        p->adjvex = endIdx;
        p->weight = w;
        p->next = G->adjlist[startIdx].firstedge;
        G->adjlist[startIdx].firstedge = p;
    }
    printf("景点拓扑图邻接表创建完成！\n");
}

void ShowVertex(Graph *G) {
    printf("全部景点信息列表：\n");
    printf("顶点序号\t景点名称\t景点简介\n");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < G->numVex; i++) {
        printf("%d\t\t%s\t\t%s\n", i, G->adjlist[i].name, G->adjlist[i].info);
    }
}

void ShowGraph(Graph *G) {
    int matrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
    int i, j;
    EdgeNode *p;

    for (i = 0; i < G->numVex; i++) {
        for (j = 0; j < G->numVex; j++) {
            matrix[i][j] = (i == j) ? 0 : INF;
        }
        for (p = G->adjlist[i].firstedge; p != NULL; p = p->next) {
            matrix[i][p->adjvex] = p->weight;
        }
    }

    printf("邻接矩阵表示：\n");
    for (i = 0; i < G->numVex; i++) {
        for (j = 0; j < G->numVex; j++) {
            if (matrix[i][j] == INF) {
                printf(" INF ");
            } else {
                printf("%4d ", matrix[i][j]);
            }
        }
        printf("\n");
    }
}

void DestroyGraph(Graph *G) {
    int i;
    EdgeNode *p, *temp;
    for (i = 0; i < G->numVex; i++) {
        p = G->adjlist[i].firstedge;
        while (p != NULL) {
            temp = p;
            p = p->next;
            free(temp);
        }
        G->adjlist[i].firstedge = NULL;
    }
    G->numVex = 0;
    G->numEdge = 0;
}