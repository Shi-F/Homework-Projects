#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define PI 3.1415926535897932384626434

typedef struct point{
	double x;
	double y;
}point;//插值点

double f(double x){
	//计算f(x) 
	return 1/(1 + x*x);
}

point *GenerateXY(int n, int mode){
    //生成n+1个插值点，返回一个point型数组 
	//mode0按等步长生成，mode1按三角函数生成 
    point *xy = (point*)malloc(sizeof(point)*(n+1));
	int i = 0;
    if(mode == 0)
    	for(i = 0;i <= n;i++)
    		xy[i].x = -5 + 10*i/double(n); 	
	else
		for(i = 0;i <= n;i++)
			xy[i].x = -5  * cos((2*i+1)*PI/double(2*n+2));
	for(i = 0;i <= n;i++)
		xy[i].y = f(xy[i].x);
	return xy;
}

double lagrange(point *xy,double xx,int n){
	//计算xx处的lagrange插值近似
	double L = 0;
	double yy;
	int i,j;
	for(i = 0;i <= n;i++){
		yy = xy[i].y;
		for(j = 0;j <= n;j++)
			if(i != j)
				yy *= (xx - xy[j].x)/(xy[i].x - xy[j].x);
		L += yy;
	}
	return L;
}

int main(){
	int n[4] = {5,10,20,40};
	int i,j;
	double max;
	double yj, err;
	point *xy = NULL;
	printf("第一组节点：\n");
	for(i = 0;i < 4;i++){
		xy = GenerateXY(n[i],0);
		//生成插值点
		max = fabs(lagrange(xy,-5,n[i])-f(-5)); 
		//取y_0时的最大模误差作为max的初始值 
		for(j = 1; j <= 500;j++){
			yj = -5 + 10 * double(j)/500;
			err = fabs(lagrange(xy,yj,n[i])-f(yj));
			if(err > max)
				max = err;
		}
		printf("	节点个数：%d\n	近似最大模误差：%.15e\n",n[i],max);
	}
	printf("第二组节点：\n");
	for(i = 0;i < 4;i++){
		xy = GenerateXY(n[i],1);
		//生成插值点
		max = fabs(lagrange(xy,-5,n[i])-f(-5));
		//取y_0时的最大模误差作为max的初始值 
		for(j = 1; j <= 500;j++){
			yj = -5 + 10 * double(j)/500;
			err = fabs(lagrange(xy,yj,n[i])-f(yj));
			if(err > max)
				max = err;
		}
		printf("	节点个数：%d\n	近似最大模误差：%.15e\n",n[i],max);
	}
	return 0;
}
