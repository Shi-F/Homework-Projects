#include<stdio.h>
#include<math.h>
#include<stdlib.h>

double function(double x){
	//定义待积函数 
	return sin(x);
}

double intfunction(double x){
	//定义原函数 
	return -cos(x);
}

void AutoInt(double a, double b, int N, double(*f)(double),double(*intf)(double)){
	//用迭代方法实现2等分的复化梯形积分和Simpson积分，取N次2等分  
	double T1, T2;	//n阶复化梯形近似、2n阶复化梯形近似
	double *S = (double*)malloc(sizeof(double)*N);
	//n阶Simpson近似，用数组保存，在最后统一输出 
	double h = b - a, e1, e2, o, H; //步长、n阶误差、2n阶误差、误差阶、新增节点的值 
	int n = 1, i, j; 
	double I = intf(b) - intf(a);	//准确的积分值
	T2 = (f(a) + f(b))*(b - a)/2;		//k = 0时为梯形近似
	e2 = I - T2;
	printf("复化梯形积分结果：\n");
	for(i = 1;i <= N;i++){
		T1 = T2;
		e1 = e2;
		H = 0;
		for(j = 0;j <= n-1;j++){
			H += f(a + (2 * j + 1) * h / 2);
		}
		H *= h;	//计算新增项和H 
		T2 = (T1 + H)/2;	//计算2n阶复化梯形近似 
		h /= 2; n *= 2;
		e2 = I - T2;	//计算2n阶误差 
		o = log(e1 / e2)/log(2);	//计算误差阶 
		printf("节点数：2^%d + 1：\n",i);
		printf("数值积分值：%.15e  ",T2);
		printf("误差：%.15e  ",e2);
		if(i != 1)
			printf("误差阶：%.15e\n",o);
		else
			printf("误差阶：/ \n",o);
		S[i-1] = (4 * T2 - T1) / 3;	//由Romberg公式得到Simpson积分 
	}
	printf("Simpson积分结果：\n");
	for(i = 0;i <= N-1;i++){
		if(i >= 1){
			printf("节点数：2^%d + 1：\n",i+1);
			printf("数值积分值：%.15e  ",S[i]);
			e1 = I - S[i-1];
			e2 = I - S[i];
			o = log(e1 / e2)/log(2);
			printf("误差：%.15e  ",e2);
			printf("误差阶：%.15e\n",o);
		}
		else{
			printf("节点数：2^%d + 1：\n",i+1);
			printf("数值积分值：%.15e  ",S[i]);
			e2 = I - S[i];
			printf("误差：%.15e  ",e2);
			printf("误差阶：/ \n");
		}
	}
}
int main(){
	double (*func)(double) = function;
	double (*intfunc)(double) = intfunction;
	AutoInt(1,5,12,func,intfunc);
	return 0;
}
