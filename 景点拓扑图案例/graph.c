/*************************************************
* 文件名：graph.c
* 功能：邻接表+DFS遍历+导游线路+回路检测+Dijkstra最短路径
* 配套文件：graph.h
**************************************************/
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 3.2.1 景点定位函数：根据名称匹配数组下标
int LocateVertex(Graph *G,char *name)
{
    int i;
    // 遍历记录的全部校园景点
    for(i=0;i<G->numVex;i++)
    {
        // 字符串匹配（精确匹配）
        if(strcmp(G->adjList[i].name,name)==0)
        {
            return i; // 匹配成功，直接返回数组下标
        }
    }
    return -1; // 匹配失败返回-1
}

// 3.2.2 创建校园景点权值邻接表
void CreateGraph(Graph *G)
{
    int i, startIdx, endIdx, k, w;
    char start[20], end[20];
    EdgeNode *p;
    char line[100];
    
    // 输入景点、道路数量
    printf("===== 输入校园景点图信息 =====\n");
    printf("请输入校园景点总数（正整数）：");
    fgets(line, sizeof(line), stdin);
    if (sscanf(line, "%d", &G->numVex) != 1) {
        printf("输入格式错误\n");
        G->numVex = 0;
        G->numEdge = 0;
        return;
    }
    printf("请输入校园连通道路总数（正整数）：");
    fgets(line, sizeof(line), stdin);
    if (sscanf(line, "%d", &G->numEdge) != 1) {
        printf("输入格式错误\n");
        G->numVex = 0;
        G->numEdge = 0;
        return;
    }

    // 记录全部景点名称+简介
    printf("\n===== 开始记录校园景点信息 =====\n");
    for(i = 0; i < G->numVex; i++)
    {
        printf("请输入第%d个景点名称：", i + 1);
        fgets(G->adjList[i].name, 20, stdin);
        G->adjList[i].name[strcspn(G->adjList[i].name, "\n")] = '\0';
        printf("请输入景点简介：");
        fgets(G->adjList[i].info, 50, stdin);
        G->adjList[i].info[strcspn(G->adjList[i].info, "\n")] = '\0';
        G->adjList[i].firstedge = NULL; // 初始化道路指针为空
    }

    // 记录校园连通道路，构建邻接表
    printf("\n===== 记录景点连通道路信息 =====\n");
    for(k = 0; k < G->numEdge; k++)
    {
        printf("请输入第%d条道路信息：起点 终点 道路距离\n", k + 1);
        fgets(line, sizeof(line), stdin);
        if (sscanf(line, "%s %s %d", start, end, &w) != 3) {
            printf("输入格式错误，请重新输入！\n");
            k--;
            continue;
        }
        startIdx = LocateVertex(G, start);
        endIdx = LocateVertex(G, end);
        if(startIdx == -1 || endIdx == -1)
        {
            printf("输入景点名称错误，请重新输入！\n");
            k--;
            continue;
        }
        // 无向图双向建边
        p = (EdgeNode*)malloc(sizeof(EdgeNode));
        p->adjvex = endIdx; p->weight = w; p->next = G->adjList[startIdx].firstedge;
        G->adjList[startIdx].firstedge = p;
        p = (EdgeNode*)malloc(sizeof(EdgeNode));
        p->adjvex = startIdx; p->weight = w; p->next = G->adjList[endIdx].firstedge;
        G->adjList[endIdx].firstedge = p;
    }
    printf("✅ 校园景点拓扑图创建成功！\n");
}

// 3.2.3 输出全部校园景点基础信息
void showVertex(Graph *G)
{
    printf("\n========== 全部校园景点信息列表 ==========\n");
    printf("编号\t景点名称\t\t景点简介\n");
    printf("--------------------------------------------------------\n");
    for(int i=0;i<G->numVex;i++)
    {
        printf("V%d\t\t%s\t\t%s\n",i,G->adjList[i].name,G->adjList[i].info);
    }
}

// 3.2.4 邻接表转邻接矩阵输出（无向图：边权=0/无边=32767）
void ShowGraph(Graph *G)
{
    int matrix[MAX_VEX][MAX_VEX];
    int i,j;
    EdgeNode *p;
    // 矩阵初始化
    for(i=0;i<G->numVex;i++)
    {
        for(j=0;j<G->numVex;j++)
        {
            if(i==j) matrix[i][j] = 0;
            else matrix[i][j] = INF;
        }
    }
    // 赋值边权值
    for(i=0;i<G->numVex;i++)
    {
        p = G->adjList[i].firstedge;
        while(p!=NULL)
        {
            matrix[i][p->adjvex] = p->weight;
            p = p->next;
        }
    }
    // 格式化输出邻接矩阵
    printf("\n========== 校园景点图邻接矩阵 ==========\n");
    printf("(对角线=0  无连通道路=32767)\n");
    for(i=0;i<G->numVex;i++)
    {
        for(j=0;j<G->numVex;j++)
        {
            printf("%8d",matrix[i][j]);
        }
        printf("\n");
    }
}

// 3.2.5 释放图动态内存
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

//==================== 上节课 DFS遍历实现（实训重点） ====================
/**
 * @brief 递归深度优先遍历 DFS
 * @param G 原始校园景点拓扑图
 * @param vexId 当前景点数组下标
 * @param visited 临时访问表：避免景点重复访问
 * @param path 输出数组：记录DFS遍历路径下标
 * @param pathLen 输出参数：路径有效景点个数，指针类型
 * @note 在任何逻辑修改前先备份，边递归边入栈，回溯时出栈
 */
void DFS(Graph *G, int vexId, int visited[], int path[], int *pathLen)
{
    // 步骤1：标记当前景点已访问，DFS核心逻辑：防止路径死循环
    visited[vexId] = 1;
    // 步骤2：将当前景点加入路径数组，实现原始路径记录
    path[(*pathLen)++] = vexId;

    // 步骤3：遍历当前景点的全部连通道路
    EdgeNode *p = G->adjList[vexId].firstedge;
    while(p != NULL)
    {
        // 判断：如果邻居还没被访问，递归访问下一条边的端点
        if(visited[p->adjvex] == 0)
        {
            DFS(G, p->adjvex, visited, path, pathLen);
        }
        // 切换下一个相邻校园景点
        p = p->next;
    }
    // 递归结束：如无未访问邻居点，自动回溯
}

/**
 * @brief 创建导游专用路线图 createGuideGraph
 * @param srcMap 原始校园景点拓扑图
 * @param guideMap 输出参数：生成的导游专用路线图
 * @param path DFS遍历生成的景点下标数组
 * @param pathLen 路径数组长度
 * @return int 返回路线图道路条数
 * @note 核心思路：只提取DFS遍历中相邻景点的边，去除全图分支边
 */
int createGuideGraph(Graph *srcMap, Graph *guideMap, int path[], int pathLen)
{
    // 边界判断：如果景点不足两个，无法生成导游路线
    if(pathLen < 2)
    {
        printf("❌ 景点数量不足，无法生成校园导游路线！\n");
        return 0;
    }
    // 1.初始化导游专用邻接表
    guideMap->numVex = srcMap->numVex;
    guideMap->numEdge = 0;
    for(int i=0; i<guideMap->numVex; i++)
    {
        // 复制全部景点信息到导游路线图
        strcpy(guideMap->adjList[i].name, srcMap->adjList[i].name);
        strcpy(guideMap->adjList[i].info, srcMap->adjList[i].info);
        guideMap->adjList[i].firstedge = NULL;
    }

    // 2.根据DFS路径数组提取相邻景点的有效边
    int edgeCount = 0;
    for(int i=0; i < pathLen-1; i++)
    {
        int now = path[i];    // 当前遍历景点
        int next = path[i+1]; // 下一站观光景点
        int roadWeight = 0;   // 两点之间道路距离

        // 查询原图两点之间的边权值
        EdgeNode *p = srcMap->adjList[now].firstedge;
        while(p && p->adjvex != next)
            p = p->next;
        if(p) roadWeight = p->weight;

        // 3.无向图双向建边，生成导游路线邻接表
        EdgeNode *e1 = (EdgeNode*)malloc(sizeof(EdgeNode));
        e1->adjvex = next; e1->weight = roadWeight;
        e1->next = guideMap->adjList[now].firstedge;
        guideMap->adjList[now].firstedge = e1;

        EdgeNode *e2 = (EdgeNode*)malloc(sizeof(EdgeNode));
        e2->adjvex = now; e2->weight = roadWeight;
        e2->next = guideMap->adjList[next].firstedge;
        guideMap->adjList[next].firstedge = e2;
        edgeCount++;
    }
    guideMap->numEdge = edgeCount;
    printf("\n✅ 校园观光路线图创建成功！\n");
    return edgeCount;
}

/**
 * @brief 格式化输出美观的校园导游观光路线
 */
void PrintGuideLine(Graph *guideMap, int path[], int pathLen)
{
    printf("==================== ✅ 校园推荐观光路线 ====================\n");
    // 循环格式化打印观光路线
    for(int i=0; i<pathLen; i++)
    {
        printf("%s",guideMap->adjList[path[i]].name);
        if(i != pathLen - 1)
            printf(" → ");
    }
    printf("\n==============================================================\n");
    printf("总路线含校园景点数量：%d \n路线图包含道路条数：%d\n",pathLen,guideMap->numEdge);
}

//==================== 本节课1：导游线路回路检测算法 ====================
/**
 * @brief 递归DFS：检测导游线路回路，标记重复景点并输出栈
 * @param guideMap 导游专用路线图
 * @param cur 当前景点数组下标
 * @param visited 全局临时访问表
 * @param inLoopPath 当前路径访问表（用于回路判断）
 * @param tempPath 临时存储当前递归路径
 * @param tempLen 临时路径有效长度
 */
void LoopCheckDFS(Graph *guideMap, int cur, int visited[], int inLoopPath[], int tempPath[], int *tempLen)
{
    // 标记全局已访问+当前路径已访问
    visited[cur] = 1;
    inLoopPath[cur] = 1;
    tempPath[(*tempLen)++] = cur;

    // 遍历当前景点的全部邻接点
    EdgeNode *p = guideMap->adjList[cur].firstedge;
    while(p != NULL)
    {
        int next = p->adjvex;
        // 邻接点未全局访问，递归继续
        if(!visited[next])
        {
            LoopCheckDFS(guideMap, next, visited, inLoopPath, tempPath, tempLen);
        }
        // 邻接点在当前路径中，说明发现回路
        else if(inLoopPath[next])
        {
            printf("\n⚠️ 发现导游线路回路！以下是重复访问的景点：\n");
            int startPos = 0;
            // 定位回路开始节点
            while(tempPath[startPos] != next)
                startPos++;
            // 循环回溯输出回路
            for(int i = startPos; i < *tempLen; i++)
            {
                printf("%s",guideMap->adjList[tempPath[i]].name);
                if(i != *tempLen - 1)
                    printf(" → ");
            }
            printf(" → %s\n",guideMap->adjList[next].name);
            printf("=========================================\n");
        }
        p = p->next;
    }
    // 回溯：出栈当前路径访问标记
    (*tempLen)--;
    inLoopPath[cur] = 0;
}

/**
 * @brief 导游线路回路检测入口函数
 */
void CheckGuideLoopLine(Graph *guideMap)
{
    int visited[VISIT_MAX] = {0};
    int inLoopPath[LOOP_PATH_MAX] = {0};
    int tempPath[LOOP_PATH_MAX] = {0};
    int tempLen = 0;

    printf("\n===== 开始检测导游线路回路与重复景点 =====\n");
    for(int i = 0; i < guideMap->numVex; i++)
    {
        if(!visited[i])
        {
            LoopCheckDFS(guideMap, i, visited, inLoopPath, tempPath, &tempLen);
        }
    }
    printf("✅ 回路检测完成，当前观光线路无闭环与重复访问景点！\n");
}

//==================== 本节课2：Dijkstra最短路径算法实现 ====================
/**
 * @brief Dijkstra单源最短路径算法
 * @param G 校园原始景点图
 * @param start 起点景点下标
 * @param dis 输出数组：记录起点到各点最短距离
 * @param visit 临时标记：记录各点是否已确定最短路径
 * @param path 输出数组：记录最短路径前驱节点
 */
void Dijkstra(Graph *G, int start, int dis[], int visit[], int path[])
{
    int n = G->numVex;
    // 1.数组初始化
    for(int i = 0; i < n; i++)
    {
        dis[i] = INF;     // 初始所有节点不可达
        visit[i] = 0;     // 初始所有节点未访问、未确定
        path[i] = -1;     // 初始无前驱节点
    }
    dis[start] = 0;        // 起点到自己距离为0

    // 2.外层循环：共执行n次，确保所有节点最短路径确定
    for(int i = 0; i < n; i++)
    {
        // 步骤1：找距离最小的未确定节点
        int u = -1;
        int minDis = INF;
        for(int j = 0; j < n; j++)
        {
            if(visit[j] == 0 && dis[j] < minDis)
            {
                minDis = dis[j];
                u = j;
            }
        }
        if(u == -1) break; // 无可访问节点，提前跳出

        // 步骤2：标记当前节点为已确定最短路径
        visit[u] = 1;

        // 步骤3：松弛操作（核心）
        EdgeNode *p = G->adjList[u].firstedge;
        while(p != NULL)
        {
            int v = p->adjvex;
            int w = p->weight;
            // 未确定节点，若转道更近则更新
            if(visit[v] == 0 && dis[v] > dis[u] + w)
            {
                dis[v] = dis[u] + w;
                path[v] = u;
            }
            p = p->next;
        }
    }
}

//==================== 本节课3：递归路径回溯输出 ====================
/**
 * @brief 递归回溯：从终点往起点打印完整最短路径
 * @param G 原图：用于获取景点名称
 * @param path 前驱路径数组
 * @param end 终点景点下标
 */
void RecursionPrintPath(Graph *G, int path[], int end)
{
    // 递归终止条件：找到起点
    if(path[end] == -1)
    {
        printf("%s",G->adjList[end].name);
        return;
    }
    // 先递归打印前驱节点
    RecursionPrintPath(G, path, path[end]);
    // 回溯后打印当前节点，实现正序路径
    printf(" → %s",G->adjList[end].name);
}




