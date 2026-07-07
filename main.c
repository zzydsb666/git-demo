#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "graph.h"


void InitGraph(ALGraph *G)
{
   
    memset(G->roadlist,0,sizeof(G->roadlist));
    
    for(int i = 0; i < MAX_VERTEX_NUM; i++)
    {
        G->edge_head[i] = NULL;
    }
    //初始空图：0个顶点，0条边
    G->nodenum = 0;
    G->edgenum = 0;
    printf("✔ 邻接表初始化完成！\n");
}

//顶点录入功能：批量写入案例校园顶点数据
void CreateGraph(ALGraph *G)
{
    //绑定结构图参数：4个顶点、4条无向边
    G->nodenum = 4;
    G->edgenum = 4;

    //逐个录入顶点名称，存入roadlist顶点数组
    strcpy(G->roadlist[0],"图书馆");
    strcpy(G->roadlist[1],"教学楼");
    strcpy(G->roadlist[2],"食堂");
    strcpy(G->roadlist[3],"宿舍");

    printf("✔ 案例顶点数据录入完成！\n");
}

//按名称查找顶点下标：遍历roadlist顶点数组匹配
int LocateVertex(ALGraph *G,VertexType name)
{
    //遍历全部有效顶点，匹配名称
    for(int i = 0; i < G->nodenum; i++)
    {
        if(strcmp(G->roadlist[i], name) == 0)
        {
            return i; //匹配成功，返回顶点数组下标
        }
    }
    return -1; //匹配失败，返回-1
}

//打印顶点信息：可视化输出邻接表基础数据
void ShowGraph(ALGraph *G)
{
    printf("\n==== 当前图顶点存储信息 ====\n");
    printf("图总顶点数：%d,总边数：%d\n",G->nodenum,G->edgenum);
    for(int i = 0; i < G->nodenum; i++)
    {
        printf("下标%d : 顶点名称【%s】\n",i,G->roadlist[i]);
    }
}

//主函数：案例测试入口
int main()
{
    //设置控制台编码为GBK，解决中文乱码
    SetConsoleOutputCP(936);
    ALGraph G; //定义整张图结构体变量
    InitGraph(&G);    //1.初始化邻接表
    CreateGraph(&G);  //2.录入案例顶点数据
    ShowGraph(&G);     //3.打印查看存储结果

    //测试顶点查找功能
    VertexType target = "食堂";
    int pos = LocateVertex(&G,target);
    if(pos != -1)
    {
        printf("✔查找结果：【%s】下标为：%d\n",target,pos);
    }
    else
    {
        printf("\n✗ 未找到该顶点\n");
    }
    return 0;
}