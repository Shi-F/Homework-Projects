#include<stdio.h>
#include<stdlib.h>
#define TRUE 1
#define FALSE 0
#define ERROR -1
#define OVERFLOW -2
#define OK 1 
#define INFINITY 2147483647 
typedef int Status;

typedef struct MGraph{
	int **arcs;
	int vexnum,arcnum;
}MGraph;

typedef struct closedge{
	int adjvex;
	int lowcost;
}closedge;

Status SetArcs(MGraph &G){
	//输入各边 
	int i,j,k;
	int right;
	for(k = 0;k < G.arcnum;k++){
		scanf("%d %d %d",&i,&j,&right);
		G.arcs[i][j] = right;
		G.arcs[j][i] = right;
	}
	return OK;
} 

int MiniSpanTree_PRIM(MGraph G){
	//从顶点1出发用Prim算法构建最小生成树
	//返回树各边的长度之和 
	closedge *close = (closedge*)malloc(sizeof(closedge)*(G.vexnum+1));
	if(!close){
		perror("内存分配失败\n");
		exit(OVERFLOW); 
	}
	int i,j,k;
	int rightsum = 0;
	int min;
	for(i = 2;i <= G.vexnum;i++){
		close[i].adjvex = 1;
		close[i].lowcost = G.arcs[1][i];
	}	//初始化close数组 
	close[1].lowcost = 0;
	for(i = 1;i < G.vexnum;i++){
		min = INFINITY;
		for(j = 1;j <= G.vexnum;j++){
			if(close[j].lowcost && close[j].lowcost < min){
				min = close[j].lowcost;
				k = j;
			}
		}
		//找到距离最短的下个顶点k 
		rightsum += G.arcs[close[k].adjvex][k];
		//增加总长
		close[k].lowcost = 0;
		//第k顶点并入已有树
		for(j = 1;j <= G.vexnum;j++){
			if(G.arcs[k][j] < close[j].lowcost){
				close[j].adjvex = k;
				close[j].lowcost = G.arcs[k][j];
			}
		} //更新close数组 
	}
	return rightsum;
}

int MiniSpanTree_KRUS(MGraph G){
	//从顶点1出发用Kruskal算法构建最小生成树
	//返回树各边的长度之和 
	int *vexgroup = (int*)malloc(sizeof(int)*(G.vexnum+1));
	if(!vexgroup){
		perror("内存分配失败\n");
		exit(OVERFLOW); 
	}	//创建顶点分组标记数组 
	int i,j,k;
	for(i = 1;i <= G.vexnum;i++){
		vexgroup[i] = i;
	}	//初始化顶点分组标记 
	int rightsum = 0;
	int min,mi,mj;
	for(k = 1;k < G.vexnum;k++){
		min = INFINITY;
		for(i = 1;i <= G.vexnum;i++){
			for(j = i+1;j <= G.vexnum;j++){
				if(vexgroup[i] != vexgroup[j] && min > G.arcs[i][j]){
					min = G.arcs[i][j];
					mi = i;mj = j;
				}
			}
		}
		//找到处于不同连通分量的最短边mi-mj 
		rightsum += min;
		//增加总长
		for(i = 1;i <= G.vexnum;i++){
			if(vexgroup[i] == vexgroup[mj])
				vexgroup[i] = vexgroup[mi];
		}
		//更新顶点分组标记 
	}
	return rightsum;
}
	
int main(){
	MGraph G;
	int i,j;
	scanf("%d %d",&G.vexnum,&G.arcnum);
	G.arcs = (int**)malloc(sizeof(int*)*(G.vexnum+1));
	if(!G.arcs){
		perror("内存分配失败\n");
		exit(OVERFLOW); 
	}
	for(i = 0;i <= G.vexnum;i++){
		G.arcs[i] = (int*)malloc(sizeof(int)*(G.vexnum+1));
		if(!G.arcs[i]){
			perror("内存分配失败\n");
			exit(OVERFLOW); 
		}
		for(j = 0;j <= G.vexnum;j++)
			G.arcs[i][j] = INFINITY;
	}
	//完成图邻接矩阵的创建与初始化 
	SetArcs(G);
	//输入边的信息
	printf("Prim算法结果为：%d\n",MiniSpanTree_PRIM(G));
	printf("Kruskal算法结果为：%d\n",MiniSpanTree_KRUS(G));
	//输出最小生成树边长度之和 
	return OK;
} 
