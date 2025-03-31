#include <stdio.h>
#include<graphics.h>
#include<conio.h>
#include<stdlib.h>
#include<math.h>
#define TRUE 1
#define FALSE 0
#define OK 1 
#define T_0 0.00001
#define X_0 100
#define K_1 0.5 
#define K_2 0.00001
typedef int Status;

typedef struct MGraph{
	int **arcs;
	int vexnum,arcnum;
}MGraph;

typedef struct Vector{
	double x;
	double y;
}Vector; 

typedef struct Vertex{
	double x;
	double y;	
	Vector v;
	Vector a;
}Vertex;

Status SetArcs(MGraph &G){
	//输入各边 
	int i,j,k;
	for(k = 0;k <G.arcnum;k++){
		scanf("%d %d",&i,&j);
		getchar();
		G.arcs[i][j] = 1;
		G.arcs[j][i] = 1;
	}
	return OK;
} 

void GetA(MGraph G,Vertex *ver){
	//计算各顶点的加速度
	int i,j;
	for(i = 1;i <= G.vexnum;i++){
		ver[i].a.x = 0;
		ver[i].a.y = 0;
		for(j = 1;j<= G.vexnum;j++){
			if(j != i){
				if(ver[j].x > ver[i].x){
					if(G.arcs[i][j] == 1)
						ver[i].a.x += K_1*(ver[j].x-ver[i].x-X_0);	//计算引力
					ver[i].a.x -= K_2/(ver[j].x-ver[i].x)/(ver[j].x-ver[i].x);	//计算斥力 
				}
				else{
					if(G.arcs[i][j] == 1)
						ver[i].a.x -= K_1*(ver[i].x-ver[j].x-X_0);	//计算引力
					ver[i].a.x += K_2/(ver[i].x-ver[j].x)/(ver[i].x-ver[j].x);	//计算斥力 
				}
				if(ver[j].y > ver[i].y){
					if(G.arcs[i][j] == 1)
						ver[i].a.y += K_1*(ver[j].y-ver[i].y-X_0);	//计算引力
					ver[i].a.y -= K_2/(ver[j].y-ver[i].y)/(ver[j].y-ver[i].y);	//计算斥力 
				}
				else{
					if(G.arcs[i][j] == 1)
						ver[i].a.y -= K_1*(ver[i].y-ver[j].y-X_0);	//计算引力
					ver[i].a.y += K_2/(ver[i].y-ver[j].y)/(ver[i].y-ver[j].y);	//计算斥力 
				} 
			}
		}
	}
}

void GetNextStatus(MGraph G,Vertex *ver){
	//计算下一时刻的速度与坐标 
	int i;
	for(i = 1;i <= G.vexnum;i++){
		ver[i].x += ver[i].v.x * T_0 + ver[i].a.x * T_0 * T_0;
		ver[i].v.x += ver[i].a.x * T_0;
		ver[i].y += ver[i].v.y * T_0 + ver[i].a.y * T_0 * T_0;
		ver[i].v.y += ver[i].a.y * T_0;
	}
}

void PrintGraph(MGraph G,Vertex *ver){
	//输出图 
	initgraph(1440,720);
	setorigin(720,360);
	setbkcolor(WHITE);
	cleardevice();
	setfillcolor(WHITE);
	setlinecolor(BLACK);
	settextcolor(BLACK);
	//完成初始化
	int i,j;
	char s[5];
	for(i = 1;i <= G.vexnum;i++){
		for(j = i;j <= G.vexnum;j++){
			if(G.arcs[i][j]){
				line(ver[i].x,ver[i].y,ver[j].x,ver[j].y);
			}
		}
	} 
	for(i = 1;i <= G.vexnum;i++){
		fillcircle(ver[i].x,ver[i].y,16);
		sprintf(s,"%d",i);
		outtextxy(ver[i].x-3,ver[i].y-7,s);
	} 
	getchar();
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
			G.arcs[i][j] = 0;
	}
	//完成图邻接矩阵的创建与初始化 
	SetArcs(G);
	//输入边的信息
	Vertex *ver = (Vertex*)malloc(sizeof(Vertex)*(G.vexnum+1));
	for(i = 1;i <= G.vexnum;i++){
		ver[i].a.x = 0;
		ver[i].a.y = 0;
		ver[i].v.x = 0;
		ver[i].v.y = 0;
		ver[i].x = 250 * cos((i-1) * 2 * 3.1415926 / G.vexnum);
		ver[i].y = 250 * sin((i-1) * 2 * 3.1415926 / G.vexnum);
	}
	//建立各顶点并初始化
	for(i = 0;i <= 1000000;i++){
		GetA(G,ver);
		GetNextStatus(G,ver);
	}
	for(i = 1;i <= G.vexnum;i++){
		printf("%d %lf %lf\n",i,ver[i].x,ver[i].y);
	}
	PrintGraph(G,ver);
	return OK;
} 
