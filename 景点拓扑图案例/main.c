/*************************************************
* 文件名：main.c
* 功能：主函数全流程调度：原图创建-DFS线路生成-回路检测-最短路径查询
**************************************************/
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ShowMenu()
{
    printf("\n=========校园导游系统菜单=========\n");
    printf("===== 数据结构实训：导游回路检测+Dijkstra最优线路规划 =====\n");
    printf("1.创建校园景点拓扑图\n");
    printf("2.查看全部景点信息\n");
    printf("3输出图邻接矩阵");
    printf("4. DFS生成基础导游观光路线\n");
    printf("5. 导游线路回路检测\n");
    printf("6. Dijkstra查询两点最短游览路线\n");
    printf("0. 退出系统并释放内存\n");
    printf("==========================================================\\n");
    printf("请输入功能编号：");

}

int main()
{
    // 设置控制台编码为 UTF-8（解决 Windows 中文乱码问题）
    system("chcp 65001 > nul");

    Graph campusMap, guideLineMap;
    // DFS基础变量
    int visited[VISIT_MAX] = {0};
    int path[PATH_MAX] = {0};
    int pathLength = 0;
    int startSpot = 0;

    // Dijkstra核心数组
    int dis[DIJ_PATH_MAX];
    int dijVisit[DIJ_PATH_MAX];
    int dijPrePath[DIJ_PATH_MAX];
    char startName[20], endName[20];
    int sIdx, eIdx;
    
    int choice;
    //标记图是否创建，防止未创建就进行操作
    int graphCreated = 0;
    //标记DFS路径是否生成
    int guidePathCreated = 0;

    printf("====数据结构实训：导游回路检测+Dijkstra最短游览路径规划====\n");
    while (1)
    { 
        ShowMenu();
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            if (graphCreated)
            {
                DestroyGraph(&campusMap);
                DestroyGraph(&guideLineMap);
                memset(visited, 0, sizeof(visited));
                memset(path, 0, sizeof(path));
                pathLength = 0;
                guidePathCreated = 0;
            }
            CreateGraph(&campusMap);
            graphCreated = 1;
            printf("校园拓扑图创建成功！\n");
            break;

        case 2:
            if (!graphCreated)
            {
                printf("请先创建校园拓扑图！\n");
                break;
            }
            showVertex(&campusMap);
            break;
        
        case 3:
            if (!graphCreated)
            {
                printf("请先创建校园拓扑图！\n");
                break;
            }
            ShowGraph(&campusMap);
            break;
            case 4:
            if (!graphCreated)
            {
                printf("请先创建校园拓扑图！\n");
                break;
            }
            memset(visited, 0, sizeof(visited));
            memset(path, 0, sizeof(path));
            pathLength = 0;
            printf("\n====执行DFS深度优先遍历====\n");
            DFS(&campusMap, startSpot, visited, path, &pathLength);
            createGuideGraph(&campusMap, &guideLineMap, path, pathLength);
            PrintGuideLine(&guideLineMap, path, pathLength);
            guidePathCreated = 1;
            break;
            case 5:
            if (!graphCreated)
            {
            printf("⚠️ 请先创建校园拓扑图！\n");
            break;
            }
            if (!guidePathCreated)
            {
            printf("⚠️ 请先执行DFS生成导游线路！\n");
            break;
            }
    printf("\n=====执行功能4后生成导游路线再检测回路!=====\n");
    CheckGuideLoopLine(&guideLineMap);
    break;

            case 6:
            if (!graphCreated)
            {
                printf("请先创建校园拓扑图！\n");
                break;
            }
            printf("\n====执行Dijkstra最短路径查询====\n");
            printf("请输入起始景点名称：");
            scanf("%s", startName);
            printf("请输入终点景点名称：");
            scanf("%s", endName);

            sIdx = LocateVertex(&campusMap, startName);
            eIdx = LocateVertex(&campusMap, endName);

            if (sIdx == -1 || eIdx == -1)
            {
                printf("输入的景点名称有误！\n");
                break;
            }
            Dijkstra(&campusMap, sIdx, dis, dijVisit, dijPrePath);
            if (dis[eIdx] == INF)
            {
                printf("无法从%s景点到%s景点！\n", startName, endName);
            }
            else
            {
                printf("最优最短游览路线：");
                RecursionPrintPath(&campusMap, dijPrePath, eIdx);
                printf("\n最短距离为：%d\n", dis[eIdx]);
            }
            break;
            case 0:
            printf("正在释放内存，退出系统...\n");
            if (graphCreated)
            {
                DestroyGraph(&campusMap);
                DestroyGraph(&guideLineMap);
            }
            printf("系统已退出！\n");
            return 0;
            default:
            printf("输入的选项有误！请重新输入！\n");
            break;
        }
    }
    return 0;
}





  