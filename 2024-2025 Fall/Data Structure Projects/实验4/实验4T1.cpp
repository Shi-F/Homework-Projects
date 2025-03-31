#include<stdio.h>
#include<stdlib.h>
#define TRUE 1
#define FALSE 0
#define ERROR -1
#define OVERFLOW -2
#define OK 1 
typedef int Status;

typedef struct MGraph{
	int **arcs;
	int vexnum,arcnum;
}MGraph;

typedef struct QNode{
	int data;
	struct QNode *next;
}QNode;

typedef struct Queue{
	QNode *front;
	QNode *rear;
}LinkQueue; 

Status SetArcs(MGraph &G){
	//输入各边 
	int i,j,k;
	for(k = 0;k <G.arcnum;k++){
		scanf("%d %d",&i,&j);
		G.arcs[i][j] = 1;
		G.arcs[j][i] = 1;
	}
	return OK;
} 

Status EnQueue(LinkQueue &Q,int data){
	QNode *p = (QNode*)malloc(sizeof(QNode));
	p->data = data;
	p->next = NULL;
	Q.rear->next = p;
	Q.rear = p;
	return OK;
}

Status DeQueue(LinkQueue &Q,int &data){
	QNode *p = Q.front->next;
	data = p->data;
	Q.front->next = p->next;	
	free(p);
	if(!Q.front->next)
		Q.rear = Q.front;
	return OK;
}

Status QueueEmpty(LinkQueue Q){
	return (Q.front == Q.rear);
}

Status DFSTraverse(MGraph G,int start){
	//深度优先搜索 
	void DFS(MGraph G,int u,int *visited);
	int *visited = (int*)malloc(sizeof(int)*(G.vexnum+1));
	if(!visited){
		perror("内存分配失败\n");
		exit(OVERFLOW); 
	} 
	int i;
	for(i = 0;i <= G.vexnum;i++)
		visited[i] = FALSE;
	//初始化访问标记数组
	DFS(G,start,visited);
	for(i = 1;i <= G.vexnum;i++){
		if(!visited[i]) DFS(G,i,visited);
	} 
	putchar('\n');
	return OK; 
}

void DFS(MGraph G,int u,int *visited){
	//深度优先搜索的递归算法
	visited[u] = TRUE;
	printf("%d ",u);
	int w;
	for(w = 1;w <= G.vexnum;w++){
		if(!visited[w] && G.arcs[u][w])
			DFS(G,w,visited);
	}
}

Status BFSTraverse(MGraph G,int start){
	void BFS(MGraph G,int u,int *visited,LinkQueue &Q);
	int *visited = (int*)malloc(sizeof(int)*(G.vexnum+1));
	if(!visited){
		perror("内存分配失败\n");
		exit(OVERFLOW); 
	} 
	int i;
	for(i = 0;i <= G.vexnum;i++)
		visited[i] = FALSE;
	//初始化访问标记数组
	LinkQueue Q;
	Q.front = (QNode*)malloc(sizeof(QNode)); 
	if(!Q.front){
		perror("内存分配失败\n");
		exit(OVERFLOW); 
	}
	Q.rear = Q.front;
	BFS(G,start,visited,Q);
	for(i = 1;i <= G.vexnum;i++){
		if(!visited[i]) BFS(G,i,visited,Q);
	} 
	return OK;
} 

void BFS(MGraph G,int u,int *visited,LinkQueue &Q){
	int w;
	visited[u] = TRUE;
	printf("%d ",u);
	EnQueue(Q,u);
	while(!QueueEmpty(Q)){
		DeQueue(Q,u);
		for(w = 1;w <= G.vexnum;w++){
			if(!visited[w] && G.arcs[u][w]){
				visited[w] = TRUE;
				printf("%d ",w);
				EnQueue(Q,w);
			}
		}
	}
}

int main(){
	MGraph G;
	int i,j,start;
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
			G.arcs[i][j] = 0;
	}
	//完成图邻接矩阵的创建与初始化 
	SetArcs(G);
	//输入边的信息
	scanf("%d",&start);
	if(start >= 1 && start <= G.vexnum){
		DFSTraverse(G,start);
		BFSTraverse(G,start);
	}
	else printf("输入不合法\n");
	return OK;
} 
