#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define OVERFLOW -2
#define MAXSIZE 1000
#define STACKMAXSIZE 100
#include<stdio.h>
#include<stdlib.h>
typedef int Status;

typedef struct{
	char *base;
	char *top;
}Stack,*SqStack;

Status InitStack(SqStack S){
	//构造空顺序栈S 
	S->base = (char*)malloc(sizeof(char)*STACKMAXSIZE);
	if(!S->base){
		printf("存储空间分配失败！\n");
		exit(OVERFLOW); 
	}
	S->top = S->base;
	return OK;
} 

Status Push(SqStack S,char c){
	//c入栈
	*S->top = c;
	S->top++;
	return OK; 
}

char GetTop(SqStack S){
	//取栈顶元素
	return *(S->top-1);
}

char Pop(SqStack S){
	//栈顶元素出栈
	char c;
	c = *(S->top-1);
	S->top--;
	return c;
}

Status BracketCmp(char a,char b){
	//判定括号a是否比括号b优先级低
	int la,lb;
	if(a == '(') la = 1;
	if(a == '[') la = 2;
	if(a == '{') la = 3;
	if(b == '(') lb = 1;
	if(b == '[') lb = 2;
	if(b == '{') lb = 3;
	if(la < lb) return TRUE;
	else return FALSE;
} 

Status BracketPairing(char a,char b){
	//判定右括号a是否与左括号b配对
	int la,lb;
	if(a == ')') la = 1;
	if(a == ']') la = 2;
	if(a == '}') la = 3;
	if(b == '(') lb = 1;
	if(b == '[') lb = 2;
	if(b == '{') lb = 3;
	if(la == lb) return TRUE;
	else return FALSE; 
}

Status CheckBracket(char *text,SqStack S){
	//检查括号是否合法，若合法返回TRUE，否则FALSE
	char *pc = text;
	while(*pc != '@'){
		if(*pc == '(' || *pc == '[' || *pc == '{'){
			//字符为左括号 
			if(S->base == S->top || BracketCmp(*pc,GetTop(S))){
				//若栈空，或该括号比栈顶括号优先级低，入栈 
				Push(S,*pc);
			}
			else return FALSE;
			//若该括号不比栈顶括号优先级低，则错误 	
		}
		if(*pc == ')' || *pc == ']' || *pc == '}'){
			//字符为右括号
			if(S->base != S->top && BracketPairing(*pc,GetTop(S))){
				//若栈非空，且括号与栈顶左括号配对，出栈 
				Pop(S); 
			}
			else return FALSE;
			//若该括号不与栈顶括号配对或栈空，则错误 
		}
		pc++;
	}
	if(S->base != S->top)
	//若此时栈不为空，则错误 
		return FALSE;
	//完成所有括号入栈与出栈，括号合法 
	return TRUE; 
} 

int main(){
	int i,num;
	char text[MAXSIZE] = {'\0'};
	SqStack S;
	int *result = NULL;//存放结果 
	S = (SqStack)malloc(sizeof(Stack));
	InitStack(S);
	scanf("%d",&num);
	result = (int*)malloc(num*sizeof(int));
	if(!result){
		printf("存储空间分配失败！\n");
		exit(OVERFLOW);
	}
	for(i = 0;i < num;i++){
		scanf("%s",text);
		result[i] = CheckBracket(text,S);
	}
	for(i = 0;i < num;i++){
		if(result[i])
			printf("YES\n");
		else 
			printf("NO\n");
	}
	return OK;
} 

