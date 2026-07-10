#ifndef GRAPH_H
#define GRAPH_H

// 全局宏定义
#define MAX_VEX 100   // 最大景点顶点数量
#define INF 32767     // 无边连通 标准无穷大数值

#define VISIT_MAX MAX_VEX // 访问标记数组最大长度
#define PATH_MAX MAX_VEX  // 路径数组最大长度

// 1.边节点结构体：存储邻接点下标、边权值、下一条边指针
typedef struct EdgeNode{
    int adjvex;          // 邻接顶点数组下标
    int weight;          // 边的权值（道路距离）
    struct EdgeNode *next; // 下一个边节点
}EdgeNode;

// 2.顶点节点结构体：存储景点名称、景点描述、第一条边指针
typedef struct VexNode{
    char name[20];       // 景点名称
    char info[50];       // 景点简单介绍
    EdgeNode *firstedge;// 指向第一条邻接边
}VexNode;

// 3.总图结构体：邻接表表头，存储顶点数、边数、顶点数组
typedef struct{
    VexNode adjList[MAX_VEX];
    int numVex;          // 当前图的顶点数（景点数量）
    int numEdge;         // 当前图的边数（道路数量）
}Graph;

// ========== 核心函数声明 ==========
// 1.创建邻接表图函数
void CreateGraph(Graph *G);
// 2.按名称定位顶点下标函数
int LocateVertex(Graph *G,char *name);
// 3.打印全部景点信息函数
void showVertex(Graph *G);
// 4.邻接表转邻接矩阵输出函数
void ShowGraph(Graph *G);
// 5.释放图内存函数
void DestroyGraph(Graph *G);
// 6.深度优先遍历函数:遍历景点，标记已访问景点，记录路径
void DFS(Graph *G,int vexId, int visited[], int path[], int *pathLen); // 深度优先遍历函数
// 7.创建导游图函数:从DFS路径提取有效边，生成新邻接表
int createGuideGraph(Graph *srcMap, Graph *guideMap, int path[], int pathLen); // 创建导游图函数
//格式化打印美观校园导游观光路线
void PrintGuideLine(Graph *guideMap, int path[], int pathLen); // 打印导游路线函数
#endif