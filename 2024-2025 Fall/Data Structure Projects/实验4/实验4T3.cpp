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

int ShortestPath_DIJ(MGraph G,int start,int end){
	//用Dijkstra算法计算start到end的最短路径
	//返回最短路径的长度 
	int *distance = (int*)malloc(sizeof(int)*(G.vexnum+1));
	if(!distance){
		perror("内存分配失败\n");
		exit(OVERFLOW); 
	}	//存储到顶点的最短距离 
	int *final = (int*)malloc(sizeof(int)*(G.vexnum+1));
	if(!final){
		perror("内存分配失败\n");
		exit(OVERFLOW); 
	}	//存储是否经过顶点 
	int i,j,k;
	int min;
	for(i = 1;i <= G.vexnum;i++){
		distance[i] = G.arcs[start][i];
		final[i] = FALSE;
	}	//初始化 
	final[start] = TRUE;
	distance[start] = 0;
	for(i = 1;i < G.vexnum;i++){
		min = INFINITY;
		for(j = 1;j <= G.vexnum;j++){
			if(!final[j] && min > distance[j]){
				k = j;
				min = distance[j];
			}
		}
		//找到未经过的最近顶点k
		final[k] = TRUE;
		//标记经过k 
		for(j = 1;j <= G.vexnum;j++){
			if(!final[j] && distance[j] > min+G.arcs[k][j]){
				distance[j] = min+G.arcs[k][j];
			}
		}
		//更新未经过顶点最近距离 
	}
	//完成各顶点最近距离计算
	return distance[end]; 
}

int main(){
	MGraph G;
	int i,j;
	int start,end;
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
	scanf("%d %d",&start,&end);
	printf("%d",ShortestPath_DIJ(G,start,end));
	return OK;
} 
