/*************************************************
* 文件名：graph.h
* 功能：图邻接表 全局宏定义+结构体+函数声明
* 承接实训：基于DFS导游线路，新增回路检测+Dijkstra最短路径
* 引用关系：graph.c、main.c 必须导入本文件
**************************************************/
#ifndef GRAPH_H
#define GRAPH_H

// 全局基础宏定义
#define MAX_VEX 100   // 校园最大景点数量
#define INF 32767     // 无边连通 标准无穷大数值

// DFS原有全局宏定义
#define VISIT_MAX MAX_VEX  // 访问标记数组最大长度
#define PATH_MAX MAX_VEX   // 导游路径数组最大长度

// 本课新增宏定义
#define LOOP_PATH_MAX MAX_VEX  // 回路检测临时路径数组长度
#define DIJ_PATH_MAX MAX_VEX   // Dijkstra路径数组最大长度

// 1.边节点结构体：存储邻接点下标、边权值、下一条边指针
typedef struct EdgeNode{
    int adjvex;            // 相邻景点数组下标
    int weight;            // 校园道路权值（道路距离）
    struct EdgeNode *next; // 下一条连通道路节点
}EdgeNode;

// 2.顶点节点结构体：存储校园景点基础信息
typedef struct VexNode{
    char name[20];         // 景点名称
    char info[50];         // 景点简单介绍
    EdgeNode *firstedge;   // 指向该景点第一条连通道路
}VexNode;

// 3.总图结构体：邻接表表头
typedef struct{
    VexNode adjList[MAX_VEX];// 全部景点数组
    int numVex;              // 当前校园景点总数
    int numEdge;             // 当前校园连通道路总数
}Graph;

// ==========基础函数声明（不变保留） ==========
void CreateGraph(Graph *G);
int LocateVertex(Graph *G,char *name);
void showVertex(Graph *G);
void ShowGraph(Graph *G);
void DestroyGraph(Graph *G);

// ========== 上节课DFS+导游线路函数（保留） ==========
void DFS(Graph *G, int vexId, int visited[], int path[], int *pathLen);
int createGuideGraph(Graph *srcMap, Graph *guideMap, int path[], int pathLen);
void PrintGuideLine(Graph *guideMap, int path[], int pathLen);

// ========== 本课新增核心函数声明 ==========
// 导游线路回路检测递归函数
void LoopCheckDFS(Graph *guideMap, int cur, int visited[], int inLoopPath[], int tempPath[], int *tempLen);
// 回路检测入口函数
void CheckGuideLoopLine(Graph *guideMap);
// Dijkstra最短路径核心算法
void Dijkstra(Graph *G, int start, int dis[], int visit[], int path[]);
// 递归回溯输出最短完整路径
void RecursionPrintPath(Graph *G, int path[], int end);

#endif
