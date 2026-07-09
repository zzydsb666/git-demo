/*************************************************
* 文件名：graph.c
* 功能：图邻接表四大核心函数具体实现
* 依赖文件：必须导入 graph.h 头文件
* 包含函数：LocateVertex、CreateGraph、showVertex、ShowGraph
**************************************************/
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//==================== 3.2.1 顶点定位函数 ====================
// 功能：按景点名称查找顶点下标，查找失败返回-1（基础工具函数）
int LocateVertex(Graph *G,char *name)
{
    int i;
    // 遍历所有已录入的景点顶点
    for(i=0;i<G->numVex;i++)
    {
        // 字符串匹配：对比输入名称和数组存储景点名
        if(strcmp(G->adjList[i].name,name)==0)
        {
            return i; // 匹配成功，返回顶点下标
        }
    }
    return -1; // 遍历完毕无匹配，返回-1
}

//==================== 3.2.2 图创建函数（重难点） ====================
// 功能：手动输入顶点、边信息，构建无向带权图邻接表
void CreateGraph(Graph *G)
{
    int i, startIdx, endIdx, k, w;
    char start[20], end[20]; // 存储输入的起点景点名、终点景点名
    EdgeNode *p;
    char line[100];
    
    // 第一步：输入图的顶点数和边数
    printf("=====请输入图信息=====\n");
    printf("请输入景点数量(顶点数)：");
    fgets(line, sizeof(line), stdin);
    if (sscanf(line, "%d", &G->numVex) != 1) {
        printf("输入格式错误！\n");
        G->numVex = 0;
        G->numEdge = 0;
        return;
    }
    printf("请输入道路数量(边数)：");
    fgets(line, sizeof(line), stdin);
    if (sscanf(line, "%d", &G->numEdge) != 1) {
        printf("输入格式错误！\n");
        G->numEdge = 0;
        return;
    }

    // 第二步：录入所有顶点（景点）名称和简介
    printf("\n=====开始录入所有景点信息=====\n");
    for(i = 0; i < G->numVex; i++)
    {
        printf("请输入第%d个景点名称：", i + 1);
        fgets(G->adjList[i].name, 20, stdin);
        // 去除换行符
        G->adjList[i].name[strcspn(G->adjList[i].name, "\n")] = '\0';
        printf("请输入该景点简介：");
        fgets(G->adjList[i].info, 50, stdin);
        // 去除换行符
        G->adjList[i].info[strcspn(G->adjList[i].info, "\n")] = '\0';
        G->adjList[i].firstedge = NULL; // 初始化边链表为空
    }

    // 第三步：录入所有带权边，构建无向图邻接表（双向建边）
    printf("\n=====录入景点连通道路及权值=====\n");
    for(k = 0; k < G->numEdge; k++)
    {
        printf("请输入第%d条道路【起点景点 终点景点 道路距离】：\n", k + 1);
        fgets(line, sizeof(line), stdin);
        // 从行中解析出起点、终点和距离
        if (sscanf(line, "%s %s %d", start, end, &w) != 3) {
            printf("输入格式错误，请重新输入！\n");
            k--;
            continue;
        }
        // 调用定位函数，获取起止顶点下标
        startIdx = LocateVertex(G, start);
        endIdx = LocateVertex(G, end);
        if(startIdx == -1 || endIdx == -1)
        {
            printf("输入景点名称错误，跳过本条道路！\n");
            k--; // 本条输入无效，重新输入
            continue;
        }

        // 无向图：创建 i→j 方向边节点，挂载到邻接表
        p = (EdgeNode*)malloc(sizeof(EdgeNode));
        if (p == NULL) {
            printf("内存分配失败！\n");
            return;
        }
        p->adjvex = endIdx;
        p->weight = w;
        p->next = G->adjList[startIdx].firstedge;
        G->adjList[startIdx].firstedge = p;

        // 无向图：创建 j→i 方向边节点（无向图双向连通）
        p = (EdgeNode*)malloc(sizeof(EdgeNode));
        if (p == NULL) {
            printf("内存分配失败！\n");
            return;
        }
        p->adjvex = startIdx;
        p->weight = w;
        p->next = G->adjList[endIdx].firstedge;
        G->adjList[endIdx].firstedge = p;
    }
    printf("? 景点拓扑图邻接表创建完成！\n");
}

//====================3.2.3 输出全部景点信息函数 ====================
void showVertex(Graph *G)
{
    printf("\n==========全部景点信息列表==========\n");
    printf("顶点编号\t景点名称\t\t景点简介\n");
    printf("--------------------------------------------------------\n");
    for(int i=0;i<G->numVex;i++)
    {
        printf("V%d\t\t%s\t\t%s\n",i,G->adjList[i].name,G->adjList[i].info);
    }
}

//==================== 3.2.4 邻接表转邻接矩阵输出函数（考点） ====================
// 输出规则：自身节点=0；无连通节点=32767
void ShowGraph(Graph *G)
{
    int matrix[MAX_VEX][MAX_VEX];
    int i,j;
    EdgeNode *p;
    // 第一步：矩阵初始化 全部赋值32767
    for(i=0;i<G->numVex;i++)
    {
        for(j=0;j<G->numVex;j++)
        {
            if(i==j) matrix[i][j] = 0; // 自身顶点对角线赋值0
            else matrix[i][j] = INF;   // 默认无连通赋值32767
        }
    }

    // 第二步：遍历邻接表链表，把权值覆盖写入邻接矩阵
    for(i=0;i<G->numVex;i++)
    {
        p = G->adjList[i].firstedge;
        while(p!=NULL)
        {
            matrix[i][p->adjvex] = p->weight;
            p = p->next;
        }
    }

    // 第三步：格式化对齐输出邻接矩阵
    printf("\n==========图邻接矩阵输出结果==========\n");
    printf("输出规则：自身节点=0  无连通节点=32767\n");
    for(i=0;i<G->numVex;i++)
    {
        for(j=0;j<G->numVex;j++)
        {
            printf("%8d",matrix[i][j]);// 统一宽度排版
        }
        printf("\n");
    }
}

//==================== 3.2.5 释放图内存函数 ====================
// 功能：释放图中所有动态分配的边节点内存
void DestroyGraph(Graph *G)
{
    int i;
    EdgeNode *p, *temp;
    for(i = 0; i < G->numVex; i++)
    {
        p = G->adjList[i].firstedge;
        while(p != NULL)
        {
            temp = p;
            p = p->next;
            free(temp);
        }
        G->adjList[i].firstedge = NULL;
    }
    G->numVex = 0;
    G->numEdge = 0;
}

void DFS(Graph *G,int vexId, int visited[], int path[], int *pathLen)
{
    visited[vexId] = 1; // 标记当前顶点已访问
    path[(*pathLen)++] = vexId; // 将当前顶点加入路径

    EdgeNode *p = G->adjList[vexId].firstedge;
    while(p != NULL)
    {
        if(!visited[p->adjvex])
        {
            DFS(G, p->adjvex, visited, path, pathLen); // 递归访问邻接顶点
        }
        p = p->next;
    }
}

int createGuideGraph(Graph *srcMap, Graph *guideMap, int path[], int pathLen)
{
    if(pathLen<2){
        printf("路径长度不足，无法创建导游图！\n");
        return 0;
    }
guideMap->numVex =srcMap->numVex;
guideMap->numEdge = 0;
for(int i=0;i<guideMap->numVex;i++){
    strcpy(guideMap->adjList[i].name,srcMap->adjList[i].name);
    strcpy(guideMap->adjList[i].info,srcMap->adjList[i].info);
    guideMap->adjList[i].firstedge = NULL;}
    int edgeCount = 0;
    for(int i=0;i<pathLen-1;i++){
        int now = path[i];
        int next = path[i+1];
        int roadWeight = 0;

        EdgeNode *p = srcMap->adjList[now].firstedge;
        while(p && p->adjvex != next){
            p = p->next;
        }
        if(p){
            roadWeight = p->weight;
        }
        EdgeNode *e1 = (EdgeNode*)malloc(sizeof(EdgeNode));
        e1->adjvex = next;
        e1->weight = roadWeight;
        e1->next = guideMap->adjList[now].firstedge;
        guideMap->adjList[now].firstedge = e1;

        EdgeNode *e2 = (EdgeNode*)malloc(sizeof(EdgeNode));
        e2->adjvex = now;
        e2->weight = roadWeight;
        e2->next = guideMap->adjList[next].firstedge;
        guideMap->adjList[next].firstedge = e2;
        edgeCount++;
    }
    guideMap->numEdge = edgeCount;
    return 1;
}

void PrintGuideLine(Graph *guideMap, int path[], int pathLen){
 printf("\n==========美观校园导游路线==========\n");
    //循环格式化打印游览路线
    for(int i=0; i<pathLen; i++)
    {
        printf("%s", guideMap->adjList[path[i]].name);
        if(i != pathLen - 1)
        {
            printf(" -> ");
        }
    }
    printf("\n=============================================\n");
    printf("线路覆盖校园景点总数： %d\n 线路通行道路总数： %d\n", pathLen, guideMap->numEdge);
}
