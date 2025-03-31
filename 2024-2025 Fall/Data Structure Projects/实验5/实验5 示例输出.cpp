#include<stdio.h>
#include<stdlib.h>
#define OK 1
#define ERROR -1
#define SUCCESS 1
#define UNSUCCESS 0
#define DUPLICATE -1
#define NULLKEY 2147483647
typedef int Status;

typedef struct CNode{
	int elem;
	struct CNode *next;
}CNode,*Chain;

typedef struct CHTable{
	Chain *ChainHash;
	int *ChainLength; 
	int count;
}CHashTable;

typedef struct HTable{
	int *elem;
	int *successtime;
	int *failtime; 
	int count;
}HashTable;

int tablesize,num;

int Hash(int x){
	//计算哈希地址
	return x % tablesize;
}

Status InitTable(HashTable &HT,CHashTable &CHT,int *data){
	int i,k,count = 0; 
	int address;
	CNode *p = NULL,*q = NULL;
	HT.elem = (int*)malloc(sizeof(int)*tablesize);
	HT.successtime = (int*)malloc(sizeof(int)*tablesize);
	HT.failtime = (int*)malloc(sizeof(int)*tablesize);
	HT.count = 0;
	CHT.ChainHash = (Chain*)malloc(sizeof(Chain)*tablesize);
	CHT.ChainLength = (int*)malloc(sizeof(int)*tablesize);
	CHT.count = 0;
	for(int i = 0;i < tablesize;i++){
		HT.elem[i] = NULLKEY; 
		HT.successtime[i] = 0;
		HT.failtime[i] = 0;
		CHT.ChainHash[i] = NULL;
		CHT.ChainLength[i] = 0;
	}
	//创建与初始化 
	for(i = 0;i < num;i++){
		count = 1;
		address = Hash(data[i]);
		//插入HT表内容 
		HT.count++;
		k = address;
		while(HT.elem[k] != NULLKEY){
			k = (k+1)%tablesize;
			count++;
		}
		HT.successtime[k] = count;
		//找到插入位置，记录成功查找次数 
		HT.elem[k] = data[i];
		//完成赋值
		//插入CHT表内容 
		CHT.count++; 
		p = (CNode*)malloc(sizeof(CNode));
		p->elem = data[i];
		p->next = NULL;
		//完成新结点创建 
		q = CHT.ChainHash[address];
		if(!q)
			CHT.ChainHash[address] = p;
		else{
			while(q->next && q->next->elem < data[i])
				q = q->next;
			p->next = q->next;
			q->next = p;
		}
		//按顺序插入结点
		CHT.ChainLength[address]++; 
	}
	for(i = 0;i < tablesize;i++){
		//计算失败查找次数
		count = 1;
		k = i;
		while(HT.elem[k] != NULLKEY){
			k = (k+1)%tablesize;
			count++;
		}
		HT.failtime[i] = count;
	} 
	return OK;
} 

Status PrintTable(HashTable HT,CHashTable CHT){
	int i,j,k,maxclen = 0;
	double TotalSTime,TotalFTime;
	CNode *p = NULL; 
	printf("HT1内容为：\n");
	printf("哈希表的地址：	");
	for(i = 0;i < tablesize;i++)
		printf("%-5d",i);
	putchar('\n');
	printf("表中的关键字：	");
	for(i = 0;i < tablesize;i++){
		if(HT.elem[i] != NULLKEY)
			printf("%-5d",HT.elem[i]);
		else
			printf("-    ");
	}
	putchar('\n');
	TotalSTime = 0;
	printf("成功查找次数：	");
	for(i = 0;i < tablesize;i++){
		printf("%-5d",HT.successtime[i]);
		TotalSTime += HT.successtime[i];
	}
	putchar('\n');
	TotalFTime = 0;
	printf("失败查找次数：	");
	for(i = 0;i < tablesize;i++){
		printf("%-5d",HT.failtime[i]);
		TotalFTime += HT.failtime[i];
	}	
	putchar('\n');
	printf("查找成功的平均查找长度：%.2lf\n",TotalSTime/num);
	printf("查找失败的平均查找长度：%.2lf\n\n",TotalFTime/tablesize);
	//输出HT1内容 
	printf("HT2内容为：\n");
	printf("哈希表的地址：	");
	for(i = 0;i < tablesize;i++)
		printf("%-5d",i);
	putchar('\n');
	printf("表中的关键字：	");
	for(i = 0;i < tablesize;i++){
		if(CHT.ChainHash[i]){
			printf("%-5d",CHT.ChainHash[i]->elem);
			if(CHT.ChainLength[i] > maxclen)
				maxclen = CHT.ChainLength[i];
		}
		else
			printf("-    ");
	}
	putchar('\n');
	for(j = 2;j <= maxclen;j++){
		printf("                ");
		for(i = 0;i < tablesize;i++){
			if(CHT.ChainLength[i] >= j){
				p = CHT.ChainHash[i];
				for(k = 1;k < j;k++)
					p = p->next;
				printf("%-5d",p->elem);
			}
			else
				printf("     ");
		}
		putchar('\n');
	}
	TotalSTime = 0;
	printf("成功查找次数：	");
	for(i = 0;i < tablesize;i++){
		if(CHT.ChainHash[i]){
			printf("1    ");
			TotalSTime += 1;
		}
		else
			printf("0    ");
	}
	putchar('\n');
	for(j = 2;j <= maxclen;j++){
		printf("                ");
		for(i = 0;i < tablesize;i++){
			if(CHT.ChainLength[i] >= j){
				printf("%-5d",j);
				TotalSTime += j;
			}
			else
				printf("     ");
		}
		putchar('\n');
	}
	TotalFTime = 0;
	printf("失败查找次数：	");
	for(i = 0;i < tablesize;i++){
		printf("%-5d",CHT.ChainLength[i]+1);
		TotalFTime += CHT.ChainLength[i]+1;
	}	
	putchar('\n');
	printf("查找成功的平均查找长度：%.2lf\n",TotalSTime/num);
	printf("查找失败的平均查找长度：%.2lf\n",TotalFTime/tablesize);		
	return OK;
}
int main(){
	FILE *fp = NULL;
	char filename[20] = {0};
	HashTable HT;
	CHashTable CHT;
	int *data = NULL; 
	int i;
	printf("请输入文件路径：");
	scanf("%s",filename);
	putchar('\n');
	if(!(fp = fopen(filename,"r"))){
		perror("文件打开失败！\n");
		exit(ERROR);
	}
	fscanf(fp,"%d",&num);
	//读取记录个数
	data = (int*)malloc(sizeof(int)*num);
	for(i = 0;i < num;i++){
		fscanf(fp,"%d",&data[i]);
	}
	//读入数据
	fscanf(fp,"%d",&tablesize);
	//读入除留余数法的模(表长) 
	InitTable(HT,CHT,data);
	PrintTable(HT,CHT);
	return OK;
}
