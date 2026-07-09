#include "graph.h"
#include <stdio.h>

#define VISIT_MAX 100

int main()
{
	Graph campusMap,guideLineMap;
	int visited[VISIT_MAX] = {0}; // 初始化访问标记数组
	int path[VISIT_MAX] = {0};    // 初始化路径数组
	int pathLength = 0;                 // 初始化路径长度
	int startSpot = 0;
	
	printf("=====数据结构实训 第四天： 校园景点DFS遍历+导游路线生成=====\n");
	CreateGraph(&campusMap); // 创建校园景点图邻接表
	showVertex(&campusMap); // 打印所有景点信息
	ShowGraph(&campusMap); // 邻接表转邻接矩阵输出

	printf("\n=====开始对校园景点DFS深度优先遍历=====\n");
	DFS(&campusMap, startSpot, visited, path, &pathLength); // 深度优先遍历景点，记录路径

	createGuideGraph(&campusMap, &guideLineMap, path, pathLength); // 从DFS路径创建导游图邻接表

	PrintGuideLine(&guideLineMap, path, pathLength); // 打印美观校园导游路线

	DestroyGraph(&campusMap); // 释放校园景点图内存
	DestroyGraph(&guideLineMap); // 释放导游图内存
	return 0;
}