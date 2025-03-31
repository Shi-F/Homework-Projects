#define OK 1
#define ERROR 0
#define OVERFLOW -2
#define MAXSIZE 500 
#include <stdio.h> 
#include <stdlib.h>
typedef int Status;

typedef struct SqStack{
	char *base;
	char *top;
	int stacksize;
}SqStack;

int strcmp(char *A,char *B){
	char *pa = A,*pb = B;
	while(*pa && *pb){
		if(*pa > *pb) return 1;
		if(*pa < *pb) return -1;
		pa++;pb++;
	}
	if(*pa && !*pb) return 1;
	if(!*pa && *pb) return -1;
	if(!*pa && !*pb) return 0;
}//字符串比较函数 

int JudgeOpe(char *ope){
	if(strcmp(ope,"Get") == 0) return 1;
	else if(strcmp(ope,"Prev") == 0) return 2;
	else if(strcmp(ope,"Next") == 0) return 3;
	else if(strcmp(ope,"Move") == 0) return 4;
	else if(strcmp(ope,"Insert") == 0) return 5;
	else if(strcmp(ope,"Delete") == 0) return 6;
	else if(strcmp(ope,"Rotate") == 0) return 7;
	else return 0;
}//判定操作内容，转换为编号 

Status Move(SqStack *PText,SqStack *NText,int num){
	int i; 
	int ssp=PText->stacksize,ssn=NText->stacksize;
	if(num < ssp){
		//需移动位置在当前光标位置之前
		//将PText中的ssp-num个元素出栈，放入NText
		for(i=0;i<ssp-num;i++){
			*NText->top = *(PText->top-1);
			*(PText->top-1) = 0;
			NText->top--;PText->top--;
			NText->stacksize++;
			PText->stacksize--;	
		}
	}
	if(num > ssp){
		//需移动位置在当前光标位置之后 
		//将NText中的num-ssp个元素出栈，放入PText
		for(i=0;i<num-ssp;i++){
			*PText->top = *(NText->top+1);
			*(NText->top-1) = 0;
			NText->top++;PText->top++;
			NText->stacksize--;
			PText->stacksize++;	
		}
	}
	return OK; 
}

Status Prev(SqStack *PText,SqStack *NText){
	//将PText栈顶元素出栈，放入NText
	*NText->top = *(PText->top-1);
	NText->top--;PText->top--;
	NText->stacksize++;
	PText->stacksize--;	  
	return OK;
}

Status Next(SqStack *PText,SqStack *NText){
	//将NText栈顶元素出栈，放入PText
	*PText->top = *(NText->top+1);
	NText->top++;PText->top++;
	NText->stacksize--;
	PText->stacksize++;	
	return OK;
}

Status Insert(SqStack *PText,SqStack *NText,FILE *fp,int num){
	//从fp开始读入字符存入PText，再将光标移动至原处
	int i;
	int start = PText->stacksize;//记录原光标位置
	fgetc(fp); 
	for(i=0;i<num;i++){
		*PText->top = fgetc(fp);
		PText->top++;
		PText->stacksize++;
	}
	Move(PText,NText,start); 
	return OK;
}

Status Get(SqStack *PText,SqStack *NText,FILE *nfp){
	//输出当前内容至文件和显示器
	int i;
	for(i=0;i<PText->stacksize;i++){
		fputc(*(PText->base+i),nfp);
		putchar(*(PText->base+i));
	}
	for(i=1;i<=NText->stacksize;i++){
		fputc(*(NText->top+i),nfp);
		putchar(*(NText->top+i));
	}
	putchar('\n');
	return OK;
}

Status Delete(SqStack *NText,int num){
	//只需将NText中num个元素出栈即可
	int i;
	for(i=0;i<num;i++){
		*(NText->top+1) = 0;
		NText->top++;
		NText->stacksize--;
	} 
	return OK;
} 

Status Rotate(SqStack *PText,SqStack *NText,int num){
	//将NText栈顶num个元素倒序放入栈内
	char *temp = NULL;
	temp = (char*)malloc(num*sizeof(char)); 
	int i;
	for(i=0;i<num;i++){
		temp[i] = *(NText->top+1);
		NText->top++;
	} 
	for(i=0;i<num;i++){
		*NText->top = temp[i];
		NText->top--;
	} 
	free(temp);
	return OK;
} 

Status main(){
	FILE *fp,*nfp;
	int OpeNum,num;
	int i,j;
	char ope[7];
	char V[MAXSIZE] = {0};//构建顺序存储空间 
	SqStack *PText,*NText;
	PText = (SqStack*)malloc(sizeof(SqStack));
	NText = (SqStack*)malloc(sizeof(SqStack));
	//栈PText存放光标前内容，栈NText存放光标后内容
	//栈PText和NText共用数组空间V 
	PText->base = V;
	PText->top = V;
	PText->stacksize = 0;
	NText->base = V + MAXSIZE - 1;
	NText->top = V + MAXSIZE - 1;
	NText->stacksize = 0;
	//初始化栈底与栈顶指针 
	fp = fopen("F:/Input/input.txt","r");
	if(fp == NULL){
		printf("打开文件失败！\n");
		exit(ERROR);
	}
	fscanf(fp,"%d",&OpeNum);
	if(OpeNum <= 0 || OpeNum > 50000){
		printf("指令数超出上限！\n");
		return ERROR;
	}
	for(i = 0;i < OpeNum;i++){
		fscanf(fp,"%s",ope);
		switch(JudgeOpe(ope)){
			case 1:
				nfp = fopen("F:/Input/output.txt","w");
				Get(PText,NText,nfp);
				break;
			case 2:
				Prev(PText,NText);
				break;	
			case 3:
				Next(PText,NText);
				break;
			case 4:
				fscanf(fp,"%d",&num);
				Move(PText,NText,num);
				break;
			case 5:
				fscanf(fp,"%d",&num);
				Insert(PText,NText,fp,num);
				break;	
			case 6:
				fscanf(fp,"%d",&num);
				Delete(NText,num);
				break;
			case 7:
				fscanf(fp,"%d",&num);
				Rotate(PText,NText,num);
				break;
			default:
				printf("非有效指令！\n");
				return ERROR; 
		}
	}
	return OK;
}
