#include <stdio.h>
#include <math.h>
double Sum(double x){
    int k;
    double sum = 0;
    for(k = 1E6 + 1; k >= 1; k--){
    	//由于截断误差要求1E-6，可证明级数从1E6+2项起的求和严格小于1E-6
		//从后往前求和可以一定程度减少大数加小数产生的误差 
        sum += 1/(k*(k+x));
    }
    return sum;
}
int main(){
	FILE *fp = fopen("程序输出.txt","w");
	double x[7] = {0.0,0.5,1.0,sqrt(2),10.0,100.0,300.0};
	int i = 0;
	for(i = 0;i < 7;i++){
		//结果用科学计数法输出，要求到小数点后十五位 
		if(i != 3){
			fprintf(fp,"x = %-5.1lf, y = %.15e\n",x[i],Sum(x[i]));
			printf("x = %-5.1lf, y = %.15e\n",x[i],Sum(x[i]));
		}
		else{
			fprintf(fp,"x = √2   , y = %.15e\n",x[i],Sum(x[i]));
			printf("x = √2   , y = %.15e\n",x[i],Sum(x[i]));
		}
	}
	fclose(fp);
    return 0;
}
